(() => {
  'use strict';

  document.getElementById('year').textContent = new Date().getFullYear();

  /* ---------------- Router: una página cada vez ---------------- */
  const routes = ['sobre-mi', 'proyectos', 'experiencia', 'contacto'];
  const titles = {
    'sobre-mi': 'Sobre mí', 'proyectos': 'Proyectos',
    'experiencia': 'Experiencia', 'contacto': 'Contacto',
  };
  const pageEls = {};
  routes.forEach(r => { pageEls[r] = document.getElementById('route-' + r); });
  const navItems = document.querySelectorAll('[data-route]');

  const burger = document.getElementById('navBurger');
  const siteNav = document.getElementById('siteNav');

  // Marcar como revelables las tarjetas y los ítems de la trayectoria
  document.querySelectorAll('.pcard, .timeline__item').forEach(el => el.classList.add('reveal'));

  const reduceMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  function revealIn(page) {
    const items = page.querySelectorAll('.reveal:not(.is-visible)');
    if (reduceMotion) { items.forEach(el => el.classList.add('is-visible')); return; }
    items.forEach((el, i) => { el.style.transitionDelay = Math.min(i, 8) * 55 + 'ms'; });
    requestAnimationFrame(() => requestAnimationFrame(() => {
      items.forEach(el => el.classList.add('is-visible'));
    }));
  }

  function currentRoute() {
    const h = location.hash.replace(/^#\/?/, '');
    return routes.includes(h) ? h : 'sobre-mi';
  }

  function showRoute(name) {
    if (!routes.includes(name)) name = 'sobre-mi';
    routes.forEach(r => pageEls[r].classList.toggle('is-active', r === name));
    navItems.forEach(a => a.classList.toggle('is-active', a.dataset.route === name));
    document.title = 'Irene Pascual — ' + titles[name];
    window.scrollTo(0, 0);
    revealIn(pageEls[name]);
    if (siteNav) siteNav.classList.remove('is-open');
    if (burger) burger.setAttribute('aria-expanded', 'false');
  }

  window.addEventListener('hashchange', () => showRoute(currentRoute()));

  /* ---------------- Menú móvil ---------------- */
  if (burger && siteNav) {
    burger.addEventListener('click', () => {
      const open = siteNav.classList.toggle('is-open');
      burger.setAttribute('aria-expanded', String(open));
    });
    siteNav.querySelectorAll('a').forEach(a => a.addEventListener('click', () => {
      siteNav.classList.remove('is-open');
      burger.setAttribute('aria-expanded', 'false');
    }));
  }

  /* ---------------- Detalles de proyecto (diálogos) ---------------- */
  document.querySelectorAll('[data-open]').forEach(btn => {
    const dialog = document.getElementById(btn.dataset.open);
    if (!dialog) return;
    btn.addEventListener('click', () => dialog.showModal());
  });
  document.querySelectorAll('dialog.pmodal').forEach(dialog => {
    dialog.querySelectorAll('[data-close]').forEach(btn => {
      btn.addEventListener('click', () => dialog.close());
    });
    dialog.addEventListener('click', (e) => {
      if (e.target === dialog) dialog.close();
    });
  });

  /* ---------------- Tilt 3D en las tarjetas ---------------- */
  const canTilt = window.matchMedia('(hover: hover) and (prefers-reduced-motion: no-preference)').matches;
  if (canTilt) {
    document.querySelectorAll('.pcard').forEach(card => {
      card.addEventListener('mouseenter', () => card.classList.add('is-tilting'));
      card.addEventListener('mousemove', (e) => {
        const r = card.getBoundingClientRect();
        const px = (e.clientX - r.left) / r.width - 0.5;
        const py = (e.clientY - r.top) / r.height - 0.5;
        card.style.transform =
          `perspective(700px) rotateX(${(-py * 5).toFixed(2)}deg) rotateY(${(px * 7).toFixed(2)}deg) translateY(-3px)`;
      });
      card.addEventListener('mouseleave', () => {
        card.classList.remove('is-tilting');
        card.style.transform = '';
      });
    });
  }

  /* ---------------- Arranque ---------------- */
  showRoute(currentRoute());

})();
