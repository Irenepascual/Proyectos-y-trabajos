# Belle Epoque Zaragoza — Le Petit Palace

Página web del salón de belleza y peluquería **Belle Epoque · Le Petit Palace**.

Es una web escaparate: presenta el salón, la carta completa de servicios con
precios, una galería de trabajos y un asistente de reserva de citas. No necesita
base de datos ni servicios externos: todo el contenido está en el propio
proyecto, así que es rápida, fácil de mantener y se puede publicar en cualquier
hosting.

Hecha con Next.js, Tailwind CSS y next-intl (español e inglés).

## Poner en marcha en local

Necesitas [Node.js](https://nodejs.org) 20 o superior.

```bash
npm install
npm run dev
```

Abre [http://localhost:3000](http://localhost:3000) en el navegador. La web se
recarga sola cada vez que guardas un cambio.

Para generar la versión final de producción:

```bash
npm run build
npm start
```

## Cómo cambiar el contenido

Todo lo editable está pensado para tocarse en un solo sitio:

- **Servicios y precios** → `src/lib/services-data.ts`
  Cada servicio tiene nombre, categoría, descripción, precio (en céntimos) y
  duración. Por ejemplo, `price_cents: 2800` son 28,00 €.

- **Fotos** → carpeta `public/images`
  Las imágenes que vienen son ilustraciones provisionales con la estética del
  salón. Sustitúyelas por fotos reales manteniendo el mismo nombre de archivo
  (por ejemplo `hero.svg` → `hero.jpg`) y actualiza la extensión donde se usan
  si cambias el formato.

- **Galería** → `src/lib/gallery-data.ts`
  Lista de imágenes de la galería con su categoría y descripción.

- **Horario, teléfono, dirección y redes** → `src/app/[locale]/page.tsx`
  (bloque de horario y contacto) y los textos en `messages/es.json` y
  `messages/en.json`.

- **Textos de la web** → `messages/es.json` (español) y `messages/en.json`
  (inglés).

## Estructura de páginas

- `/` — Inicio: presentación, servicios destacados, horario y contacto.
- `/es/servicios` — Carta completa de servicios por categorías.
- `/es/galeria` — Galería de trabajos.
- `/es/reservar` — Asistente de reserva de cita paso a paso.

Las mismas páginas están en inglés en `/en/...`.

## Publicar la web

La forma más sencilla es [Vercel](https://vercel.com):

1. Sube este repositorio a tu cuenta de GitHub.
2. En Vercel, elige **Add New → Project** e importa el repositorio.
3. Vercel detecta Next.js automáticamente. Pulsa **Deploy**.

En pocos minutos tendrás una dirección pública. También puedes conectar tu
propio dominio desde el panel de Vercel.
