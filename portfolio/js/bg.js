/* Fondo aurora formal: manchas de azul profundo y gris pizarra
   derivando muy lentamente detrás del contenido, con un único
   destello cálido apenas perceptible (canvas 2D, sin dependencias). */
(() => {
  'use strict';

  const canvas = document.getElementById('bgfx');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const reduced = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  // r/x/y en fracciones del viewport; dx/dy amplitud de deriva; sp velocidad
  const BLOBS = [
    { c: '36,72,107',   a: .11, r: .60, x: .12, y: .12, dx: .06, dy: .05, sp: .000060, ph: 0.0 },
    { c: '74,85,104',   a: .10, r: .55, x: .85, y: .30, dx: .05, dy: .06, sp: .000050, ph: 2.1 },
    { c: '90,110,140',  a: .08, r: .58, x: .68, y: .85, dx: .06, dy: .05, sp: .000068, ph: 4.2 },
    { c: '52,74,96',    a: .08, r: .50, x: .25, y: .74, dx: .05, dy: .06, sp: .000044, ph: 1.3 },
    { c: '201,150,43',  a: .05, r: .38, x: .55, y: .45, dx: .05, dy: .05, sp: .000055, ph: 3.4 },
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
