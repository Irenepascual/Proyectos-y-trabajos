(() => {
  'use strict';

  const prefersReducedMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  document.getElementById('year').textContent = new Date().getFullYear();

  /* ---------------- Mobile nav ---------------- */
  const burger = document.getElementById('navBurger');
  const links = document.querySelector('.nav__links');
  if (burger && links) {
    burger.addEventListener('click', () => {
      const open = links.classList.toggle('nav__links--open');
      burger.setAttribute('aria-expanded', String(open));
      links.style.display = open ? 'flex' : '';
      if (open) {
        links.style.position = 'fixed';
        links.style.top = '64px';
        links.style.left = '0';
        links.style.right = '0';
        links.style.flexDirection = 'column';
        links.style.background = 'var(--paper)';
        links.style.padding = '1.5rem 1.25rem';
        links.style.borderBottom = '1px solid var(--line)';
      }
    });
    links.querySelectorAll('a').forEach(a => a.addEventListener('click', () => {
      links.style.display = '';
      burger.setAttribute('aria-expanded', 'false');
    }));
  }

  /* ---------------- Scroll reveal ---------------- */
  const revealEls = document.querySelectorAll('.reveal');
  if ('IntersectionObserver' in window && !prefersReducedMotion) {
    const io = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          entry.target.classList.add('is-visible');
          io.unobserve(entry.target);
        }
      });
    }, { threshold: 0.15, rootMargin: '0px 0px -40px 0px' });
    revealEls.forEach(el => io.observe(el));
  } else {
    revealEls.forEach(el => el.classList.add('is-visible'));
  }

  /* ---------------- Hero role rotator ---------------- */
  const rotator = document.getElementById('rotator');
  const roles = ['agentes de IA', 'grafos de conocimiento', 'pipelines de datos', 'sistemas distribuidos', 'interfaces limpias'];
  if (rotator && !prefersReducedMotion) {
    let i = 0;
    setInterval(() => {
      i = (i + 1) % roles.length;
      rotator.style.opacity = 0;
      setTimeout(() => {
        rotator.textContent = roles[i];
        rotator.style.opacity = 1;
      }, 250);
    }, 2600);
    rotator.style.transition = 'opacity .25s ease';
  }

  /* ---------------- Typewriter helper ---------------- */
  function typeLines(el, lines, opts = {}) {
    const speed = opts.speed || 18;
    const lineDelay = opts.lineDelay || 260;
    el.textContent = '';
    if (prefersReducedMotion) {
      el.innerHTML = lines.join('\n');
      return;
    }
    let li = 0;
    function nextLine() {
      if (li >= lines.length) {
        const cursor = document.createElement('span');
        cursor.className = 'cursor';
        el.appendChild(cursor);
        return;
      }
      const line = lines[li];
      let ci = 0;
      const lineNode = document.createElement('div');
      el.appendChild(lineNode);
      const tick = setInterval(() => {
        lineNode.textContent = line.slice(0, ci + 1);
        ci++;
        if (ci >= line.length) {
          clearInterval(tick);
          li++;
          setTimeout(nextLine, lineDelay);
        }
      }, speed);
    }
    nextLine();
  }

  const heroLines = [
    '$ whoami',
    'Irene Pascual Albericio — Ingeniería Informática, UZ',
    '',
    '$ cat especialidad.txt',
    'Sistemas de la Información',
    '',
    '$ ./estado.sh',
    '[OK] 4o curso — todas las asignaturas superadas',
    '[OK] TFG: chatbot IA sobre datos abiertos',
    '[..] buscando primera oportunidad profesional',
  ];
  const heroBody = document.getElementById('terminalBody');
  const traceLines = [
    '> "¿Qué colegios públicos hay cerca de Teruel?"',
    '',
    '[Supervisor]     intencion: consulta_geografica       OK  0.4s',
    '[KGQA / ReAct]   buscando esquema isonomico relevante  OK  1.1s',
    '[KGQA / ReAct]   generando consulta SPARQL             OK  3.8s',
    '[Geo]            resolviendo "Teruel" -> poligono WGS84 OK  0.9s',
    '[Virtuoso]       ejecutando sobre 50M+ tripletas        OK  12.6s',
    '[Visualizador]   formato de salida: tabla                OK  0.6s',
    '',
    'Traza completa registrada en Langfuse — EU AI Act, art. 50',
  ];
  const traceBody = document.getElementById('traceBody');

  function startWhenVisible(el, lines, opts) {
    if (!el) return;
    if (!('IntersectionObserver' in window)) { typeLines(el, lines, opts); return; }
    const io = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          typeLines(el, lines, opts);
          io.disconnect();
        }
      });
    }, { threshold: 0.3 });
    io.observe(el);
  }

  startWhenVisible(heroBody, heroLines, { speed: 16, lineDelay: 220 });
  startWhenVisible(traceBody, traceLines, { speed: 14, lineDelay: 320 });

  /* ---------------- Hero knowledge-graph canvas ---------------- */
  const canvas = document.getElementById('graphCanvas');
  if (canvas) {
    const ctx = canvas.getContext('2d');
    let w, h, nodes;
    const NODE_COUNT = 26;
    const LINK_DIST = 150;

    function resize() {
      const hero = canvas.parentElement;
      w = canvas.width = hero.offsetWidth;
      h = canvas.height = hero.offsetHeight;
    }

    function initNodes() {
      nodes = Array.from({ length: NODE_COUNT }, () => ({
        x: Math.random() * w,
        y: Math.random() * h,
        vx: (Math.random() - 0.5) * 0.25,
        vy: (Math.random() - 0.5) * 0.25,
        r: Math.random() * 1.6 + 1.2,
      }));
    }

    function step() {
      ctx.clearRect(0, 0, w, h);
      nodes.forEach(n => {
        n.x += n.vx; n.y += n.vy;
        if (n.x < 0 || n.x > w) n.vx *= -1;
        if (n.y < 0 || n.y > h) n.vy *= -1;
      });
      for (let i = 0; i < nodes.length; i++) {
        for (let j = i + 1; j < nodes.length; j++) {
          const a = nodes[i], b = nodes[j];
          const d = Math.hypot(a.x - b.x, a.y - b.y);
          if (d < LINK_DIST) {
            ctx.strokeStyle = `rgba(156,107,116,${0.16 * (1 - d / LINK_DIST)})`;
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.moveTo(a.x, a.y);
            ctx.lineTo(b.x, b.y);
            ctx.stroke();
          }
        }
      }
      nodes.forEach(n => {
        ctx.beginPath();
        ctx.arc(n.x, n.y, n.r, 0, Math.PI * 2);
        ctx.fillStyle = 'rgba(184,147,90,0.55)';
        ctx.fill();
      });
      if (!prefersReducedMotion) requestAnimationFrame(step);
    }

    resize();
    initNodes();
    step();
    window.addEventListener('resize', () => { resize(); initNodes(); });
  }
})();
