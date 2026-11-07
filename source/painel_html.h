// Gerado a partir de painel-audios.html
static const char PAINEL_HTML[] = R"HTMLPAINEL(<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<title>Painel AUDIOS</title>
<style>
:root{--bg:#e9ebef;--fg:#1b1f27;--muted:#5d6675;box-sizing:border-box;padding-top:env(safe-area-inset-top,0px);padding-bottom:env(safe-area-inset-bottom,0px)}
@media (prefers-color-scheme:dark){:root:not([data-theme="light"]){--bg:#0b0d12;--fg:#e8ebf2;--muted:#8d97a8}}
:root[data-theme="dark"]{--bg:#0b0d12;--fg:#e8ebf2;--muted:#8d97a8}
html{scroll-padding-top:env(safe-area-inset-top,0px)}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--fg);font-family:system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;display:flex;flex-direction:column;align-items:center;gap:16px;padding:20px 12px}
h1{font-size:15px;letter-spacing:.2em;margin:0;font-weight:600;color:var(--muted)}
.cluster{width:100%;max-width:980px;background:radial-gradient(ellipse at 50% 0%,#1d2330,#0a0c11 70%);border:2px solid #2a3140;border-radius:28px;padding:22px 18px 18px;box-shadow:0 20px 50px rgba(0,0,0,.45),inset 0 0 40px rgba(0,0,0,.6);color:#e8ebf2}
.row{display:grid;grid-template-columns:1fr 1.1fr 1fr;gap:10px;align-items:center}
.g{width:100%;height:auto;display:block}
.tick{stroke:#5b6578;stroke-width:2}.tick.m{stroke:#c9d2e3;stroke-width:3}
.num{fill:#9aa6bb;font-size:13px;text-anchor:middle;font-family:system-ui,sans-serif}
.arc{fill:none;stroke:#1d2433;stroke-width:10;stroke-linecap:round}
.needle{transform-origin:100px 100px;transition:transform .5s cubic-bezier(.3,1.3,.5,1)}
.needle.fast{transition:transform 1s ease-in-out}
.hub{fill:#222a3a;stroke:#3a4458;stroke-width:2}
.glab{fill:#aab4c8;font-size:11px;text-anchor:middle;letter-spacing:.15em}
.gval{fill:#fff;font-size:26px;text-anchor:middle;font-weight:600;font-variant-numeric:tabular-nums}
.center{text-align:center;display:flex;flex-direction:column;gap:10px;align-items:center}
.screen{width:100%;min-height:150px;border-radius:16px;background:#06080c;border:1px solid #232b3a;display:flex;flex-direction:column;align-items:center;justify-content:center;padding:12px;transition:box-shadow .3s,border-color .3s}
.screen small{color:#7d889c;letter-spacing:.2em;font-size:11px}
.cls{font-size:clamp(34px,8vw,60px);font-weight:700;letter-spacing:.08em;margin:6px 0;text-transform:uppercase;transition:color .3s}
.conf{font-size:14px;color:#9aa6bb;font-variant-numeric:tabular-nums}
.screen.off .cls,.screen.off small,.screen.off .conf{opacity:0}
.screen.ajuda{border-color:#ff3b30;box-shadow:0 0 28px rgba(255,59,48,.45)}.screen.ajuda .cls{color:#ff5148}
.screen.ok{border-color:#2ee66b;box-shadow:0 0 24px rgba(46,230,107,.35)}.screen.ok .cls{color:#38f27a}
.screen.ask{border-color:#3a8fe0;box-shadow:0 0 24px rgba(90,176,255,.4)}.screen.ask .cls{color:#5ab0ff;font-size:clamp(26px,6vw,44px)}
.screen.noresp{border-color:#ffb020;animation:bl 1s infinite}.screen.noresp .cls{color:#ffb84d;font-size:clamp(26px,6vw,44px)}
@keyframes bl{50%{box-shadow:0 0 4px rgba(255,176,32,.1)}0%,100%{box-shadow:0 0 30px rgba(255,176,32,.6)}}
.l.blink.on{animation:bl 1s infinite}
.gdead{opacity:.28}
.screen.vazio{border-color:#ffb020;box-shadow:0 0 18px rgba(255,176,32,.25)}.screen.vazio .cls{color:#ffb84d}
.lights{display:flex;flex-wrap:wrap;gap:8px;justify-content:center;margin-top:16px}
.l{display:flex;align-items:center;gap:6px;padding:6px 11px;border-radius:999px;font-size:11px;letter-spacing:.12em;font-weight:600;color:#303848;background:#10141c;border:1px solid #1d2433;transition:all .25s}
.l i{font-style:normal;font-size:14px}
.l.on.red{color:#ff5148;border-color:#ff3b30;background:rgba(255,59,48,.13);box-shadow:0 0 14px rgba(255,59,48,.6)}
.l.on.amb{color:#ffb84d;border-color:#ffb020;background:rgba(255,176,32,.12);box-shadow:0 0 14px rgba(255,176,32,.5)}
.l.on.grn{color:#38f27a;border-color:#2ee66b;background:rgba(46,230,107,.12);box-shadow:0 0 14px rgba(46,230,107,.5)}
.l.on.blu{color:#5ab0ff;border-color:#3a8fe0;background:rgba(90,176,255,.12);box-shadow:0 0 14px rgba(90,176,255,.5)}
.ctrl{display:flex;flex-wrap:wrap;gap:10px;justify-content:center}
button{font:inherit;font-size:14px;font-weight:600;padding:11px 18px;border-radius:12px;border:1px solid var(--muted);background:transparent;color:var(--fg);cursor:pointer}
button:hover:not(:disabled){background:rgba(128,140,160,.18)}
button:disabled{opacity:.35;cursor:not-allowed}
#ign{background:#e2362b;border-color:#e2362b;color:#fff;min-width:150px}
#ign.run{background:#1f9d4d;border-color:#1f9d4d}
.hint{font-size:12px;color:var(--muted);text-align:center;max-width:620px;margin:0}
.net{font-size:13px;color:var(--muted);text-align:center;font-variant-numeric:tabular-nums}
@media(max-width:640px){.row{grid-template-columns:1fr 1fr}.center{grid-column:1/-1;order:-1}}
</style>
</head>
<body>
<h1>PAINEL · AUDIOS</h1>
<div class="cluster">
  <div class="row">
    <svg class="g gdead" viewBox="0 0 200 200" id="gL" aria-label="Velocímetro desligado"></svg>
    <div class="center">
      <div class="screen off" id="screen"><small id="sm">PAINEL</small><div class="cls" id="cls">—</div><div class="conf" id="conf"></div></div>
    </div>
    <svg class="g gdead" viewBox="0 0 200 200" id="gR" aria-label="Conta-giros desligado"></svg>
  </div>
  <div class="lights" id="lights"></div>
</div>
<div class="ctrl">
  <button id="ign">⏻ LIGAR</button>
  <button id="ask" disabled>Perguntar (Espaço)</button>
  <button class="sim" data-c="ajuda" disabled>AJUDA (A)</button>
  <button class="sim" data-c="ok" disabled>OK (O)</button>
  <label style="font-size:13px;color:var(--muted);display:flex;align-items:center;gap:6px"><input type="checkbox" id="voz" checked>Voz</label>
</div>
<div class="net" id="net">○ Programa: desconectado</div>
<p class="hint">Os ponteiros ficam sempre apagados. Aperte <b>Espaço</b> e o painel pergunta a situação. <b>AJUDA</b> acende a luz vermelha e ela só apaga com <b>OK</b>. Sem resposta (silêncio ou classe vazio), o painel repete a pergunta. As teclas A e O simulam o modelo; no real, chame <code>painel.classificar("ajuda", 0.93)</code>.</p>
<script>
const $=id=>document.getElementById(id),sleep=ms=>new Promise(r=>setTimeout(r,ms));
function build(svg,max,step,label,lab){
  let h='<path class="arc" d="M 29.3 170.7 A 100 100 0 1 1 170.7 170.7" transform="scale(.9) translate(11,11)"/>';
  for(let v=0;v<=max;v+=step/2){
    const a=(-135+270*v/max)*Math.PI/180,m=(v%step===0),r1=m?78:84,r2=92;
    h+=`<line class="tick ${m?'m':''}" x1="${(100+r1*Math.sin(a)).toFixed(1)}" y1="${(100-r1*Math.cos(a)).toFixed(1)}" x2="${(100+r2*Math.sin(a)).toFixed(1)}" y2="${(100-r2*Math.cos(a)).toFixed(1)}"/>`;
    if(m)h+=`<text class="num" x="${(100+64*Math.sin(a)).toFixed(1)}" y="${(104-64*Math.cos(a)).toFixed(1)}">${v}</text>`;
  }
  h+=`<text class="glab" x="100" y="128">${label}</text><g style="transform:rotate(-135deg);transform-origin:100px 100px"><polygon points="97,100 100,22 103,100" fill="#555d6e"/></g><circle class="hub" cx="100" cy="100" r="9"/>`;
  svg.innerHTML=h;
}
build($('gL'),100,20,'KM/H');build($('gR'),8,1,'x1000 RPM');
const LIGHTS=[['ENGINE','⚙','amb'],['BAT','🔋','red'],['OIL','🛢','red'],['ABS','◉','amb'],['AIRBAG','☺','red'],['CINTO','⚑','red'],
 ['MIC','🎙','blu'],['AJUDA','⚠','red'],['OK','✔','grn'],['SEM RESPOSTA','⏱','amb']];
$('lights').innerHTML=LIGHTS.map(([t,i,c])=>`<div class="l ${c}" data-t="${t}"><i>${i}</i>${t}</div>`).join('');
const L=t=>document.querySelector(`.l[data-t="${t}"]`);
const lamp=(t,on)=>L(t).classList.toggle('on',on);
const allL=on=>document.querySelectorAll('.l').forEach(e=>e.classList.toggle('on',on));
const TIMEOUT=8000;
let st='off',tries=0,timer=null;
function show(cls,sm,txt,sub){$('screen').className='screen '+cls;$('sm').textContent=sm;$('cls').textContent=txt;$('conf').textContent=sub||''}
function falar(t){
  if(!$('voz').checked||!window.speechSynthesis)return;
  try{speechSynthesis.cancel();const u=new SpeechSynthesisUtterance(t);u.lang='pt-BR';speechSynthesis.speak(u)}catch(e){}
}
function standby(){
  st='standby';clearTimeout(timer);tries=0;
  ['AJUDA','OK','SEM RESPOSTA','MIC'].forEach(t=>{lamp(t,false);L(t).classList.remove('blink')});
  show('vazio','PAINEL','PRONTO','Aperte Espaço para perguntar');
  $('screen').className='screen off';$('screen').classList.remove('off');
  $('screen').style.removeProperty('border-color');
}
function perguntar(){
  if(st==='off'||st==='boot')return;
  clearTimeout(timer);
  const q=tries===0?'Qual a sua situação?':'Sem resposta. Qual a sua situação?';
  st='asking';tries++;
  lamp('MIC',true);lamp('OK',false);
  if(tries>1){lamp('SEM RESPOSTA',true);L('SEM RESPOSTA').classList.add('blink')}
  show(tries>1?'noresp':'ask','PERGUNTA',tries>1?'SEM RESPOSTA':'QUAL SUA SITUAÇÃO?',tries>1?`Tentativa ${tries} · responda AJUDA ou OK`:'Responda AJUDA ou OK');
  falar(q);
  timer=setTimeout(()=>{if(st==='asking')perguntar()},TIMEOUT);
}
function classificar(c,p){
  c=String(c).toLowerCase();
  if(st==='asking'||st==='ajuda'||(st==='standby'&&c==='ajuda')){
    if(c==='ajuda'){
      clearTimeout(timer);st='ajuda';tries=0;
      lamp('SEM RESPOSTA',false);L('SEM RESPOSTA').classList.remove('blink');lamp('MIC',false);
      lamp('AJUDA',true);L('AJUDA').classList.add('blink');
      show('ajuda','ALERTA','AJUDA!','Precisa de ajuda · diga OK para encerrar');
      falar('Ajuda acionada.');
    }else if(c==='ok'){
      clearTimeout(timer);st='ok';
      ['AJUDA','SEM RESPOSTA','MIC'].forEach(t=>{lamp(t,false);L(t).classList.remove('blink')});
      lamp('OK',true);show('ok','SITUAÇÃO','TUDO BEM','Painel normalizado');
      falar('Tudo bem.');
      setTimeout(()=>{if(st==='ok')standby()},2500);
    }
  }
}
async function ligar(){
  st='boot';$('ign').disabled=true;$('ign').textContent='INICIANDO…';
  show('vazio','PAINEL','AUTOTESTE','');allL(true);
  await sleep(1800);allL(false);
  $('ign').textContent='⏻ DESLIGAR';$('ign').classList.add('run');$('ign').disabled=false;
  $('ask').disabled=false;document.querySelectorAll('.sim').forEach(b=>b.disabled=false);
  standby();
}
function desligar(){
  st='off';clearTimeout(timer);try{speechSynthesis.cancel()}catch(e){}
  allL(false);document.querySelectorAll('.l').forEach(e=>e.classList.remove('blink'));
  show('off','PAINEL','—','');
  $('ign').textContent='⏻ LIGAR';$('ign').classList.remove('run');
  $('ask').disabled=true;document.querySelectorAll('.sim').forEach(b=>b.disabled=true);
}
$('ign').onclick=()=>st==='off'?ligar():desligar();
$('ask').onclick=perguntar;
document.querySelectorAll('.sim').forEach(b=>b.onclick=()=>classificar(b.dataset.c,0.9));
document.addEventListener('keydown',e=>{
  if(e.target.tagName==='INPUT'&&e.target.type!=='checkbox')return;
  const k=e.key.toLowerCase();
  if(k===' '){e.preventDefault();if(st==='standby'||st==='ajuda')perguntar()}
  else if(k==='a')classificar('ajuda');
  else if(k==='o')classificar('ok');
});
window.painel={ligar,desligar,perguntar,classificar};
if(location.protocol==='https:')$('net').textContent='Modo demonstração · sem ligação com o programa (use as teclas A e O)';
// Ligação com o programa C++: só quando a página é aberta por localhost
if(location.protocol!=='https:'){
  const net=$('net'),hora=()=>new Date().toLocaleTimeString('pt-BR');
  const BASE=(location.port==='8080')?'':'http://127.0.0.1:8080';
  const es=new EventSource(BASE+'/events');
  es.onopen=()=>{net.textContent='● Programa conectado · aguardando comando de voz'};
  es.onerror=()=>{net.textContent='○ Programa desconectado · abra o app.exe e recarregue a página (F5)'};
  es.onmessage=e=>{const [c,p]=e.data.split(',');net.textContent='● Programa conectado · último comando ouvido: '+c.toUpperCase()+' ('+Math.round(parseFloat(p)*100)+'%) às '+hora();classificar(c,parseFloat(p))};
}
</script>
</body>
</html>
)HTMLPAINEL";
