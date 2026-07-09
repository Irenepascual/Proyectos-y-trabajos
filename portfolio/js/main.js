(() => {
  'use strict';

  document.getElementById('year').textContent = new Date().getFullYear();

  /* ---------------- Mobile sidebar toggle ---------------- */
  const burger = document.getElementById('navBurger');
  const sidebarBody = document.getElementById('sidebarBody');
  if (burger && sidebarBody) {
    burger.addEventListener('click', () => {
      const open = sidebarBody.classList.toggle('is-open');
      burger.setAttribute('aria-expanded', String(open));
    });
    sidebarBody.querySelectorAll('.nav__links a').forEach(a => a.addEventListener('click', () => {
      sidebarBody.classList.remove('is-open');
      burger.setAttribute('aria-expanded', 'false');
    }));
  }

  /* ---------------- Scrollspy: highlight current section in nav ---------------- */
  const navAnchors = document.querySelectorAll('.nav__links a[data-nav]');
  const sections = Array.from(navAnchors)
    .map(a => document.getElementById(a.dataset.nav))
    .filter(Boolean);

  if ('IntersectionObserver' in window && sections.length) {
    const byId = {};
    navAnchors.forEach(a => { byId[a.dataset.nav] = a; });

    const io = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        const link = byId[entry.target.id];
        if (!link) return;
        if (entry.isIntersecting) {
          navAnchors.forEach(a => a.classList.remove('is-active'));
          link.classList.add('is-active');
        }
      });
    }, { rootMargin: '-45% 0px -50% 0px', threshold: 0 });

    sections.forEach(s => io.observe(s));
  }

  /* ---------------- Nav focus/spotlight highlight (vertical) ---------------- */
  const navGlow = document.getElementById('navGlow');
  const navLinks = document.getElementById('navLinks');
  if (navGlow && navLinks) {
    navAnchors.forEach(a => {
      a.addEventListener('mouseenter', () => {
        const linkBox = a.getBoundingClientRect();
        const parentBox = navLinks.getBoundingClientRect();
        navGlow.style.top = (linkBox.top - parentBox.top) + 'px';
        navGlow.style.height = linkBox.height + 'px';
        navGlow.style.opacity = '1';
      });
    });
    navLinks.addEventListener('mouseleave', () => { navGlow.style.opacity = '0'; });
  }

  /* ---------------- Project detail dialogs ---------------- */
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

  /* ---------------- Scroll-reveal ---------------- */
  const revealTargets = document.querySelectorAll(
    '.section__kicker, .section__title, .section__lede, .pcard, .timeline__item, .stat'
  );
  if ('IntersectionObserver' in window && revealTargets.length) {
    revealTargets.forEach((el, i) => {
      el.classList.add('reveal');
      el.style.transitionDelay = (i % 3) * 70 + 'ms';
    });
    const revealIO = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          entry.target.classList.add('is-visible');
          revealIO.unobserve(entry.target);
        }
      });
    }, { threshold: 0.15, rootMargin: '0px 0px -40px 0px' });
    revealTargets.forEach(el => revealIO.observe(el));
  } else {
    revealTargets.forEach(el => el.classList.add('is-visible'));
  }

  /* ---------------- Count-up numbers ---------------- */
  const counters = document.querySelectorAll('.bignum__num[data-count]');
  const noMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;
  if (counters.length && 'IntersectionObserver' in window && !noMotion) {
    const runCount = (el) => {
      const target = parseInt(el.dataset.count, 10);
      const suffix = el.dataset.suffix || '';
      const dur = 1300;
      const t0 = performance.now();
      const tick = (now) => {
        const p = Math.min((now - t0) / dur, 1);
        const eased = 1 - Math.pow(1 - p, 3);
        el.textContent = Math.round(target * eased) + suffix;
        if (p < 1) requestAnimationFrame(tick);
      };
      requestAnimationFrame(tick);
    };
    const countIO = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          runCount(entry.target);
          countIO.unobserve(entry.target);
        }
      });
    }, { threshold: 0.5 });
    counters.forEach(el => countIO.observe(el));
  }

  /* ---------------- 3D tilt on project cards ---------------- */
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

})();
