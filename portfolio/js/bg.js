/* Fondo "flujo de datos": columnas de glifos y términos de datos en gris
   oscuro derivando muy lentamente hacia arriba, sobre un lavado pizarra
   apenas perceptible (canvas 2D, sin dependencias). */
(() => {
  'use strict';

  const canvas = document.getElementById('bgfx');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const reduced = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  const GLYPHS = '01{}[]<>/=+*#$%&;:~^'.split('');
  const WORDS = ['SQL', 'RAG', 'ETL', 'RDF', 'API', 'JSON', 'SPARQL', '0x3F', '1010', 'NULL', 'SELECT', 'JOIN'];
  const INK = '44,49,56'; // gris oscuro

  // lavado de profundidad
  const WASH = [
    { c: '36,72,107', a: .09, r: .60, x: .15, y: .15, dx: .05, dy: .04, sp: .00005, ph: 0.0 },
    { c: '74,85,104', a: .08, r: .55, x: .82, y: .75, dx: .04, dy: .05, sp: .00004, ph: 2.5 },
  ];

  let w, h, dpr, cols = [];

  function rand(min, max) { return min + Math.random() * (max - min); }
  function pick(arr) { return arr[Math.floor(Math.random() * arr.length)]; }

  function buildColumns() {
    cols = [];
    const spacing = 96 * dpr;
    const n = Math.ceil(w / spacing) + 1;
    for (let i = 0; i < n; i++) {
      const cells = [];
      const step = rand(30, 42) * dpr;
      const count = Math.ceil(h / step) + 2;
      for (let j = 0; j < count; j++) {
        cells.push({
          text: Math.random() < .15 ? pick(WORDS) : pick(GLYPHS),
          a: rand(.06, .15),
          size: rand(11, 15),
        });
      }
      cols.push({
        x: i * spacing + rand(-18, 18) * dpr,
        offset: rand(0, h),
        speed: rand(14, 28) * dpr, // px por segundo, hacia arriba
        step, cells,
      });
    }
  }

  function resize() {
    dpr = Math.min(window.devicePixelRatio || 1, 1.5);
    w = canvas.width = Math.round(window.innerWidth * dpr);
    h = canvas.height = Math.round(window.innerHeight * dpr);
    buildColumns();
  }

  function draw(t) {
    ctx.clearRect(0, 0, w, h);

    const m = Math.max(w, h);
    for (const b of WASH) {
      const x = (b.x + Math.cos(t * b.sp + b.ph) * b.dx) * w;
      const y = (b.y + Math.sin(t * b.sp * 1.3 + b.ph) * b.dy) * h;
      const g = ctx.createRadialGradient(x, y, 0, x, y, b.r * m);
      g.addColorStop(0, `rgba(${b.c},${b.a})`);
      g.addColorStop(1, `rgba(${b.c},0)`);
      ctx.fillStyle = g;
      ctx.fillRect(0, 0, w, h);
    }

    ctx.textBaseline = 'top';
    for (const col of cols) {
      const total = col.cells.length * col.step;
      const shift = (col.offset + t / 1000 * col.speed) % total;
      for (let j = 0; j < col.cells.length; j++) {
        const cell = col.cells[j];
        let y = j * col.step - shift;
        if (y < -col.step) y += total;
        if (y > h) continue;
        ctx.font = `${Math.round(cell.size * dpr)}px ui-monospace, Menlo, Consolas, monospace`;
        ctx.fillStyle = `rgba(${INK},${cell.a})`;
        ctx.fillText(cell.text, col.x, y);
      }
    }
  }

  resize();
  window.addEventListener('resize', () => { resize(); if (reduced) draw(0); });

  if (reduced) { draw(0); return; }

  let raf, last = 0;
  const loop = (t) => {
    // limitar a ~30 fps: suficiente para una deriva lenta
    if (t - last >= 33) { draw(t); last = t; }
    raf = requestAnimationFrame(loop);
  };
  document.addEventListener('visibilitychange', () => {
    if (document.hidden) cancelAnimationFrame(raf);
    else raf = requestAnimationFrame(loop);
  });
  raf = requestAnimationFrame(loop);
})();
