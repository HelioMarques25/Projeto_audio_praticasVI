// Classificacao de audio ao vivo (microfone) com modelo Edge Impulse
// Com filtro de confirmacao e trava: o label detectado fica fixo por 3 segundos.
// Painel de carro no navegador: http://localhost:8080  (o comando confirmado vai para o painel)
// Usa miniaudio (arquivo unico: source/miniaudio.h) para capturar o microfone.

#include "httplib.h"   // precisa vir antes de windows.h
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "painel_html.h"

// ---- Ajustes ----
static const float LIMIAR_CONFIANCA = 0.85f;  // confianca minima para valer (0.0 a 1.0)
static const float LIMIAR_VOLUME    = 400.0f; // volume minimo (RMS) para considerar que alguem falou
static const int   INTERVALO_MS     = 250;    // de quanto em quanto tempo classifica
static const int   CONFIRMACOES     = 2;      // janelas seguidas iguais (2 = ~0,5 s) para confirmar
static const int   HOLD_MS          = 3000;   // tempo que o label fica fixo (3 s)
static const int   PORTA_PAINEL     = 8080;   // painel em http://localhost:8080

// Labels considerados "fundo": nao disparam a trava de 3 s.
static bool is_fundo(const char *label) {
    return strcmp(label, "ruido") == 0 || strcmp(label, "vazio") == 0;
}

// ---- Painel: envia o comando confirmado para o navegador (SSE) ----
static std::mutex ev_mutex;
static std::condition_variable ev_cv;
static unsigned long long ev_seq = 0;
static std::string ev_last;

static void publicar(const char *classe, float conf) {
    char b[96];
    snprintf(b, sizeof b, "%s,%.2f", classe, conf);
    { std::lock_guard<std::mutex> lk(ev_mutex); ev_last = b; ev_seq++; }
    ev_cv.notify_all();
}

static void iniciar_painel() {
    static httplib::Server svr;
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(PAINEL_HTML, sizeof(PAINEL_HTML) - 1, "text/html; charset=utf-8");
    });
    svr.Get("/events", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Cache-Control", "no-cache");
        res.set_header("Access-Control-Allow-Origin", "*");   // painel pode estar em outra origem (arquivo, Live Server)
        auto visto = std::make_shared<unsigned long long>(0);
        { std::lock_guard<std::mutex> lk(ev_mutex); *visto = ev_seq; }
        res.set_chunked_content_provider("text/event-stream",
            [visto](size_t, httplib::DataSink &sink) {
                std::string msg;
                {
                    std::unique_lock<std::mutex> lk(ev_mutex);
                    bool novo = ev_cv.wait_for(lk, std::chrono::seconds(10),
                                               [&] { return ev_seq != *visto; });
                    if (novo) { *visto = ev_seq; msg = "data: " + ev_last + "\n\n"; }
                    else msg = ": ping\n\n";
                }
                return sink.write(msg.data(), msg.size());
            });
    });
    if (!svr.bind_to_port("127.0.0.1", PORTA_PAINEL)) {
        printf("ERRO: a porta %d ja esta em uso por outro programa. Feche-o e rode de novo.\n", PORTA_PAINEL);
        return;
    }
    std::thread([] { svr.listen_after_bind(); }).detach();
}

// ---- Buffer circular com o ultimo 1 segundo de audio ----
static const size_t N = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
static std::vector<int16_t> ring(N, 0);
static size_t ring_pos = 0;
static size_t ring_filled = 0;
static std::mutex ring_mutex;

static std::vector<float> features(N, 0.0f);

static int get_signal_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features.data() + offset, length * sizeof(float));
    return 0;
}

static void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frame_count) {
    (void)device; (void)output;
    const int16_t *in = (const int16_t *)input;
    std::lock_guard<std::mutex> lock(ring_mutex);
    for (ma_uint32 i = 0; i < frame_count; i++) {
        ring[ring_pos] = in[i];
        ring_pos = (ring_pos + 1) % N;
    }
    ring_filled += frame_count;
    if (ring_filled > N) ring_filled = N;
}

int main() {
    using Clock = std::chrono::steady_clock;

    printf("Modelo: %d amostras por janela, %d Hz\n",
           (int)N, (int)EI_CLASSIFIER_FREQUENCY);

    ma_device_config cfg = ma_device_config_init(ma_device_type_capture);
    cfg.capture.format   = ma_format_s16;
    cfg.capture.channels = 1;
    cfg.sampleRate       = (ma_uint32)EI_CLASSIFIER_FREQUENCY;
    cfg.dataCallback     = data_callback;

    ma_device device;
    if (ma_device_init(NULL, &cfg, &device) != MA_SUCCESS) {
        printf("ERRO: nao consegui abrir o microfone.\n");
        return 1;
    }
    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("ERRO: nao consegui iniciar a captura.\n");
        ma_device_uninit(&device);
        return 1;
    }

    iniciar_painel();
    printf("Painel: abra http://localhost:%d no Chrome ou Edge\n", PORTA_PAINEL);

#ifdef _WIN32
    {
        // Habilita codigos ANSI (mover cursor / limpar linha) no console do Windows
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(h, &mode)) SetConsoleMode(h, mode | 0x0004);
    }
#endif
    printf("Escutando... fale ou faca o som. Ctrl+C para sair.\n\n");
    printf("\n\n");  // reserva as 2 linhas da tela (ao vivo + comando fixo)

    signal_t signal;
    signal.total_length = N;
    signal.get_data = &get_signal_data;

    // Estado do filtro e da trava
    std::string candidato;                 // label que esta sendo confirmado
    int streak = 0;                        // quantas janelas seguidas deram o mesmo label
    std::string held_label;                // label fixo
    Clock::time_point held_until = Clock::now();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(INTERVALO_MS));

        double soma_quadrados = 0.0;
        {
            std::lock_guard<std::mutex> lock(ring_mutex);
            if (ring_filled < N) continue;  // ainda enchendo o primeiro segundo
            for (size_t i = 0; i < N; i++) {
                float v = (float)ring[(ring_pos + i) % N];
                features[i] = v;
                soma_quadrados += (double)v * (double)v;
            }
        }
        float volume = (float)sqrt(soma_quadrados / (double)N);  // RMS da janela
        bool volume_ok = volume >= LIMIAR_VOLUME;

        ei_impulse_result_t result = { 0 };
        EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
        if (err != EI_IMPULSE_OK) {
            printf("Erro no classificador: %d\n", err);
            continue;
        }

        size_t best = 0;
        for (size_t i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
            if (result.classification[i].value > result.classification[best].value) best = i;
        }
        const char *best_label = result.classification[best].label;
        float best_val = result.classification[best].value;
        bool confiante = best_val >= LIMIAR_CONFIANCA;

        Clock::time_point agora = Clock::now();
        bool holding = agora < held_until;

        // Filtro: so conta se for confiante, nao for "fundo" e repetir CONFIRMACOES vezes
        if (confiante && volume_ok && !is_fundo(best_label)) {
            if (candidato == best_label) streak++;
            else { candidato = best_label; streak = 1; }
        } else {
            candidato.clear();
            streak = 0;
        }

        // Trava: confirmou e nao esta travado -> fixa o label por HOLD_MS e avisa o painel
        if (!holding && streak >= CONFIRMACOES) {
            held_label = candidato;
            held_until = agora + std::chrono::milliseconds(HOLD_MS);
            holding = true;
            publicar(held_label.c_str(), best_val);
            candidato.clear();
            streak = 0;
        }

        // Sobe 2 linhas e redesenha as duas telas
        printf("\x1b[2A");

        // Linha 1: AO VIVO (o que ele esta ouvindo agora)
        printf("\r\x1b[2K");
        if (confiante) {
            printf("AO VIVO : %-8s (%3.0f%%) | ", best_label, best_val * 100.0f);
        } else {
            printf("AO VIVO : %-8s        | ", "---");
        }
        for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
            printf("%s:%3.0f%% ", result.classification[i].label,
                   result.classification[i].value * 100.0f);
        }
        printf("| vol:%5.0f", volume);
        printf("\n");

        // Linha 2: COMANDO FIXO (fica 3 s com a palavra detectada)
        printf("\r\x1b[2K");
        if (holding) {
            long restante = (long)std::chrono::duration_cast<std::chrono::milliseconds>(held_until - agora).count();
            const int total = 20;
            int cheios = (int)(restante * total / HOLD_MS);
            if (cheios < 0) cheios = 0;
            if (cheios > total) cheios = total;
            printf("COMANDO : %-8s [", held_label.c_str());
            for (int i = 0; i < total; i++) putchar(i < cheios ? '#' : '-');
            printf("] %.1fs", restante / 1000.0);
        } else {
            printf("COMANDO : ---      (aguardando)");
        }
        printf("\n");
        fflush(stdout);
    }

    ma_device_uninit(&device);
    return 0;
}
