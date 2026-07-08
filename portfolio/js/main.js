(() => {
  'use strict';

  document.getElementById('year').textContent = new Date().getFullYear();

  /* ---------------- Mobile nav ---------------- */
  const burger = document.getElementById('navBurger');
  const links = document.getElementById('navLinks');
  if (burger && links) {
    burger.addEventListener('click', () => {
      const open = links.classList.toggle('nav__links--open');
      burger.setAttribute('aria-expanded', String(open));
      links.style.display = open ? 'flex' : '';
      if (open) {
        links.style.position = 'fixed';
        links.style.top = '57px';
        links.style.left = '0';
        links.style.right = '0';
        links.style.flexDirection = 'column';
        links.style.background = '#fff';
        links.style.padding = '1rem 1.25rem';
        links.style.borderBottom = '1px solid var(--line)';
      }
    });
    links.querySelectorAll('a').forEach(a => a.addEventListener('click', () => {
      links.style.display = '';
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

  /* ---------------- Nav focus/spotlight highlight ---------------- */
  const navGlow = document.getElementById('navGlow');
  if (navGlow && links) {
    navAnchors.forEach(a => {
      a.addEventListener('mouseenter', () => {
        const linkBox = a.getBoundingClientRect();
        const parentBox = links.getBoundingClientRect();
        navGlow.style.left = (linkBox.left - parentBox.left - 8) + 'px';
        navGlow.style.width = (linkBox.width + 16) + 'px';
        navGlow.style.opacity = '1';
      });
    });
    links.addEventListener('mouseleave', () => { navGlow.style.opacity = '0'; });
  }

  /* ---------------- Interactive skill chips ---------------- */
  const skillChips = document.querySelectorAll('.skill-chip');
  const containers = document.querySelectorAll('.feature, .project-row');
  let activeChip = null;

  function clearSkillFilter() {
    document.body.classList.remove('skills-filtering');
    containers.forEach(el => el.classList.remove('is-match'));
    if (activeChip) activeChip.classList.remove('is-active');
    activeChip = null;
  }

  skillChips.forEach(chip => {
    chip.addEventListener('click', () => {
      if (activeChip === chip) { clearSkillFilter(); return; }
      if (activeChip) activeChip.classList.remove('is-active');
      activeChip = chip;
      chip.classList.add('is-active');
      document.body.classList.add('skills-filtering');
      const key = chip.textContent.trim().toLowerCase();
      containers.forEach(el => {
        const bits = el.querySelectorAll('.badges span, .project-row__stack');
        const text = Array.from(bits).map(b => b.textContent).join(' ').toLowerCase();
        el.classList.toggle('is-match', text.includes(key));
      });
    });
  });
})();
