/* Fondo aurora: manchas de color de la paleta derivando lentamente
   detrás de todo el contenido (canvas 2D, sin dependencias). */
(() => {
  'use strict';

  const canvas = document.getElementById('bgfx');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const reduced = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  // r/x/y en fracciones del viewport; dx/dy amplitud de deriva; sp velocidad
  const BLOBS = [
    { c: '36,72,107',  a: .13, r: .55, x: .10, y: .15, dx: .09, dy: .07, sp: .00011, ph: 0.0 },
    { c: '201,150,43', a: .12, r: .48, x: .88, y: .25, dx: .08, dy: .09, sp: .00009, ph: 2.1 },
    { c: '31,127,176', a: .10, r: .52, x: .72, y: .88, dx: .07, dy: .08, sp: .00013, ph: 4.2 },
    { c: '122,59,59',  a: .09, r: .44, x: .22, y: .78, dx: .08, dy: .07, sp: .00008, ph: 1.3 },
    { c: '47,111,94',  a: .08, r: .40, x: .50, y: .48, dx: .09, dy: .08, sp: .00010, ph: 3.4 },
  ];

  let w, h;
  function resize() {
    const dpr = Math.min(window.devicePixelRatio || 1, 1.5);
    w = canvas.width = Math.round(window.innerWidth * dpr);
    h = canvas.height = Math.round(window.innerHeight * dpr);
  }

  function draw(t) {
    ctx.clearRect(0, 0, w, h);
    const m = Math.max(w, h);
    for (const b of BLOBS) {
      const x = (b.x + Math.cos(t * b.sp + b.ph) * b.dx) * w;
      const y = (b.y + Math.sin(t * b.sp * 1.3 + b.ph) * b.dy) * h;
      const g = ctx.createRadialGradient(x, y, 0, x, y, b.r * m);
      g.addColorStop(0, `rgba(${b.c},${b.a})`);
      g.addColorStop(1, `rgba(${b.c},0)`);
      ctx.fillStyle = g;
      ctx.fillRect(0, 0, w, h);
    }
  }

  resize();
  window.addEventListener('resize', () => { resize(); if (reduced) draw(0); });

  if (reduced) { draw(0); return; }

  let raf;
  const loop = (t) => { draw(t); raf = requestAnimationFrame(loop); };
  document.addEventListener('visibilitychange', () => {
    if (document.hidden) cancelAnimationFrame(raf);
    else raf = requestAnimationFrame(loop);
  });
  raf = requestAnimationFrame(loop);
})();
