/* ─── Galería de trabajos ─── */
/* Para añadir o cambiar imágenes: deja la foto en public/images y añade
   aquí una entrada nueva con su categoría y una breve descripción. */

export type GalleryItem = {
  id: string;
  image: string;
  category: string;
  description: string;
};

export const GALLERY_ITEMS: GalleryItem[] = [
  { id: "1", image: "/images/galeria-1.svg", category: "Color",      description: "Color y luminosidad" },
  { id: "2", image: "/images/galeria-2.svg", category: "Peinados",   description: "Recogido para evento" },
  { id: "3", image: "/images/galeria-3.svg", category: "Novias",     description: "Peinado de novia" },
  { id: "4", image: "/images/galeria-4.svg", category: "Maquillaje", description: "Maquillaje de invitada" },
  { id: "5", image: "/images/galeria-5.svg", category: "Color",      description: "Mechas balayage" },
  { id: "6", image: "/images/galeria-6.svg", category: "Uñas",       description: "Manicura semipermanente" },
  { id: "7", image: "/images/galeria-7.svg", category: "Estética",   description: "Tratamiento facial" },
  { id: "8", image: "/images/galeria-8.svg", category: "Peinados",   description: "Peinado de día" },
];
