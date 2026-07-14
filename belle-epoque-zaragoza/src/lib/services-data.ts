/* ─── Catálogo de servicios ─── */

export type Service = {
  id: string;
  name: string;
  category: string;
  description: string;
  price_cents: number;
  duration_minutes: number;
  sort_order: number;
};

export const CATEGORY_ORDER = [
  "peluqueria",
  "estetica_facial",
  "depilacion_laser",
  "depilacion_cera",
  "maquillaje",
  "manos_pies",
  "corporal",
] as const;

export const CATEGORY_LABELS: Record<string, string> = {
  peluqueria:       "Peluquería",
  estetica_facial:  "Estética Facial",
  depilacion_laser: "Depilación Láser Diodo",
  depilacion_cera:  "Depilación Tradicional",
  maquillaje:       "Maquillaje",
  manos_pies:       "Manos y Pies",
  corporal:         "Corporal",
};

function makeId(category: string, sortOrder: number): string {
  return `${category}_${String(sortOrder).padStart(2, "0")}`;
}

const raw: Omit<Service, "id">[] = [
  // ─── PELUQUERÍA ───
  { name: "Corte mujer",                    category: "peluqueria",       description: "Corte personalizado con asesoramiento de estilo.",                        price_cents: 2800,  duration_minutes: 45,  sort_order: 1  },
  { name: "Corte hombre",                   category: "peluqueria",       description: "Corte clásico o moderno con acabado.",                                    price_cents: 1800,  duration_minutes: 30,  sort_order: 2  },
  { name: "Corte niño/a",                   category: "peluqueria",       description: "Corte infantil hasta 12 años.",                                           price_cents: 1400,  duration_minutes: 30,  sort_order: 3  },
  { name: "Lavar y peinar",                 category: "peluqueria",       description: "Lavado con productos premium y peinado.",                                 price_cents: 1800,  duration_minutes: 30,  sort_order: 4  },
  { name: "Peinado / recogido",             category: "peluqueria",       description: "Peinado o recogido para eventos.",                                        price_cents: 4500,  duration_minutes: 60,  sort_order: 5  },
  { name: "Peinado de novia",               category: "peluqueria",       description: "Recogido o peinado de novia con prueba previa incluida.",                 price_cents: 11000, duration_minutes: 90,  sort_order: 6  },
  { name: "Tinte raíz",                     category: "peluqueria",       description: "Retoque de raíz con color personalizado.",                                price_cents: 4000,  duration_minutes: 75,  sort_order: 7  },
  { name: "Tinte completo",                 category: "peluqueria",       description: "Color integral con productos de alta gama.",                              price_cents: 5800,  duration_minutes: 90,  sort_order: 8  },
  { name: "Mechas tradicionales",           category: "peluqueria",       description: "Mechas con papel de aluminio.",                                           price_cents: 8000,  duration_minutes: 120, sort_order: 9  },
  { name: "Balayage",                       category: "peluqueria",       description: "Técnica de iluminación natural y degradada.",                             price_cents: 11000, duration_minutes: 150, sort_order: 10 },
  { name: "Babylights",                     category: "peluqueria",       description: "Mechas finas y sutiles tipo \"kissed by the sun\".",                      price_cents: 12500, duration_minutes: 150, sort_order: 11 },
  { name: "Decoloración total",             category: "peluqueria",       description: "Decoloración completa con tratamiento reparador.",                       price_cents: 10000, duration_minutes: 180, sort_order: 12 },
  { name: "Tratamiento capilar express",    category: "peluqueria",       description: "Mascarilla intensiva en cabina.",                                        price_cents: 2000,  duration_minutes: 30,  sort_order: 13 },
  { name: "Spa capilar",                    category: "peluqueria",       description: "Ritual completo de hidratación y masaje craneal.",                        price_cents: 4500,  duration_minutes: 60,  sort_order: 14 },
  { name: "Botox capilar",                  category: "peluqueria",       description: "Reconstrucción profunda para cabellos dañados.",                         price_cents: 7000,  duration_minutes: 90,  sort_order: 15 },
  { name: "Keratina",                       category: "peluqueria",       description: "Alisado y nutrición intensa de larga duración.",                         price_cents: 14000, duration_minutes: 180, sort_order: 16 },
  { name: "Alisado brasileño",              category: "peluqueria",       description: "Alisado profesional de larga duración.",                                 price_cents: 20000, duration_minutes: 240, sort_order: 17 },

  // ─── ESTÉTICA FACIAL ───
  { name: "Peeling Hollywood",                   category: "estetica_facial", description: "El tratamiento de las estrellas. Rejuvenecimiento facial con resultados visibles desde la primera sesión.", price_cents: 8500,  duration_minutes: 60,  sort_order: 1  },
  { name: "Limpieza facial básica",              category: "estetica_facial", description: "Limpieza profunda con vapor y extracción.",                                                                price_cents: 4000,  duration_minutes: 60,  sort_order: 2  },
  { name: "Limpieza facial profunda",            category: "estetica_facial", description: "Limpieza completa con mascarilla específica y masaje.",                                                    price_cents: 6500,  duration_minutes: 75,  sort_order: 3  },
  { name: "Peeling químico",                     category: "estetica_facial", description: "Renovación celular controlada según tipo de piel.",                                                       price_cents: 7500,  duration_minutes: 60,  sort_order: 4  },
  { name: "Tratamiento antiedad",                category: "estetica_facial", description: "Protocolo personalizado contra signos de la edad.",                                                       price_cents: 8000,  duration_minutes: 75,  sort_order: 5  },
  { name: "Radiofrecuencia facial",              category: "estetica_facial", description: "Reafirmación y luminosidad inmediata.",                                                                   price_cents: 7000,  duration_minutes: 60,  sort_order: 6  },
  { name: "HIFU facial",                         category: "estetica_facial", description: "Lifting no invasivo con ultrasonido focalizado.",                                                        price_cents: 19000, duration_minutes: 90,  sort_order: 7  },
  { name: "Microneedling",                       category: "estetica_facial", description: "Inducción de colágeno con microagujas.",                                                                  price_cents: 10500, duration_minutes: 75,  sort_order: 8  },
  { name: "Diseño de cejas",                     category: "estetica_facial", description: "Perfilado profesional según morfología facial.",                                                          price_cents: 1500,  duration_minutes: 30,  sort_order: 9  },
  { name: "Tinte de cejas",                      category: "estetica_facial", description: "Coloración natural con tinte específico.",                                                                price_cents: 1200,  duration_minutes: 20,  sort_order: 10 },
  { name: "Tinte de pestañas",                   category: "estetica_facial", description: "Coloración duradera para realzar la mirada.",                                                             price_cents: 1500,  duration_minutes: 30,  sort_order: 11 },
  { name: "Lifting de pestañas",                 category: "estetica_facial", description: "Curvado y nutrición de pestañas naturales.",                                                              price_cents: 4500,  duration_minutes: 60,  sort_order: 12 },
  { name: "Extensiones de pestañas pelo a pelo", category: "estetica_facial", description: "Aplicación profesional pelo a pelo.",                                                                     price_cents: 7500,  duration_minutes: 120, sort_order: 13 },

  // ─── DEPILACIÓN LÁSER DIODO ───
  { name: "Láser — Labio superior",    category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 1800, duration_minutes: 10, sort_order: 1  },
  { name: "Láser — Mentón",            category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 1800, duration_minutes: 10, sort_order: 2  },
  { name: "Láser — Axilas",            category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 3000, duration_minutes: 15, sort_order: 3  },
  { name: "Láser — Ingles bikini",     category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 2800, duration_minutes: 15, sort_order: 4  },
  { name: "Láser — Ingles completas",  category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 4500, duration_minutes: 20, sort_order: 5  },
  { name: "Láser — Facial completo",   category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 4000, duration_minutes: 20, sort_order: 6  },
  { name: "Láser — Medias piernas",    category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 5500, duration_minutes: 30, sort_order: 7  },
  { name: "Láser — Piernas completas", category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 7500, duration_minutes: 45, sort_order: 8  },
  { name: "Láser — Brazos completos",  category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 5500, duration_minutes: 30, sort_order: 9  },
  { name: "Láser — Abdomen",           category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 3000, duration_minutes: 20, sort_order: 10 },
  { name: "Láser — Glúteos",           category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 4500, duration_minutes: 20, sort_order: 11 },
  { name: "Láser — Espalda completa",  category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 7000, duration_minutes: 30, sort_order: 12 },
  { name: "Láser — Pecho completo",    category: "depilacion_laser", description: "Depilación láser diodo de última generación.", price_cents: 5500, duration_minutes: 30, sort_order: 13 },

  // ─── DEPILACIÓN TRADICIONAL (cera) ───
  { name: "Cera — Cejas",              category: "depilacion_cera", description: "Depilación con cera tibia.", price_cents: 1000, duration_minutes: 15, sort_order: 1 },
  { name: "Cera — Labio",              category: "depilacion_cera", description: "Depilación con cera tibia.", price_cents: 700,  duration_minutes: 10, sort_order: 2 },
  { name: "Cera — Ingles",             category: "depilacion_cera", description: "Depilación con cera tibia.", price_cents: 1500, duration_minutes: 20, sort_order: 3 },
  { name: "Cera — Medias piernas",     category: "depilacion_cera", description: "Depilación con cera tibia.", price_cents: 2000, duration_minutes: 30, sort_order: 4 },
  { name: "Cera — Piernas completas",  category: "depilacion_cera", description: "Depilación con cera tibia.", price_cents: 3000, duration_minutes: 45, sort_order: 5 },

  // ─── MAQUILLAJE ───
  { name: "Maquillaje de día",                    category: "maquillaje", description: "Look natural para día a día o reuniones.",                      price_cents: 4000,  duration_minutes: 45, sort_order: 1 },
  { name: "Maquillaje de noche / evento",         category: "maquillaje", description: "Look intenso para eventos y celebraciones.",                    price_cents: 5500,  duration_minutes: 60, sort_order: 2 },
  { name: "Maquillaje de invitada",               category: "maquillaje", description: "Maquillaje profesional para invitadas a bodas y eventos.",      price_cents: 6000,  duration_minutes: 60, sort_order: 3 },
  { name: "Maquillaje de madrina",                category: "maquillaje", description: "Maquillaje de larga duración para madrinas.",                   price_cents: 7000,  duration_minutes: 60, sort_order: 4 },
  { name: "Maquillaje de novia",                  category: "maquillaje", description: "Servicio completo de novia con prueba previa incluida.",        price_cents: 13000, duration_minutes: 90, sort_order: 5 },
  { name: "Prueba de maquillaje de novia",        category: "maquillaje", description: "Sesión de prueba previa a la boda.",                            price_cents: 5000,  duration_minutes: 60, sort_order: 6 },
  { name: "Maquillaje a domicilio (suplemento)",  category: "maquillaje", description: "Suplemento por desplazamiento al domicilio del cliente.",       price_cents: 2500,  duration_minutes: 0,  sort_order: 7 },

  // ─── MANOS Y PIES ───
  { name: "Manicura básica",           category: "manos_pies", description: "Limado, cutícula y esmalte tradicional.",         price_cents: 1800, duration_minutes: 30, sort_order: 1 },
  { name: "Manicura semipermanente",   category: "manos_pies", description: "Esmaltado semipermanente de larga duración.",     price_cents: 2500, duration_minutes: 45, sort_order: 2 },
  { name: "Pedicura básica",           category: "manos_pies", description: "Limado, cutícula y esmalte tradicional.",         price_cents: 2500, duration_minutes: 45, sort_order: 3 },
  { name: "Pedicura semipermanente",   category: "manos_pies", description: "Pedicura completa con semipermanente.",           price_cents: 3200, duration_minutes: 60, sort_order: 4 },
  { name: "Pedicura spa",              category: "manos_pies", description: "Ritual de exfoliación, masaje e hidratación.",    price_cents: 4500, duration_minutes: 75, sort_order: 5 },

  // ─── CORPORAL ───
  { name: "Masaje relajante",          category: "corporal", description: "Masaje corporal con aceites esenciales.",           price_cents: 5000, duration_minutes: 60, sort_order: 1 },
  { name: "Presoterapia",              category: "corporal", description: "Drenaje linfático con presión secuencial.",         price_cents: 4000, duration_minutes: 45, sort_order: 2 },
  { name: "Radiofrecuencia corporal",  category: "corporal", description: "Reafirmación y reducción de flacidez.",             price_cents: 6500, duration_minutes: 60, sort_order: 3 },
  { name: "Cavitación",                category: "corporal", description: "Reducción localizada por ultrasonidos.",            price_cents: 7000, duration_minutes: 60, sort_order: 4 },
  { name: "Maderoterapia",             category: "corporal", description: "Drenaje y modelado con instrumentos de madera.",    price_cents: 6000, duration_minutes: 60, sort_order: 5 },
];

export const SERVICES: Service[] = raw.map((s) => ({
  ...s,
  id: makeId(s.category, s.sort_order),
}));

/** Devuelve los servicios agrupados por categoría, en el orden de presentación */
export function getGroupedServices(): Record<string, Service[]> {
  const grouped: Record<string, Service[]> = {};
  for (const cat of CATEGORY_ORDER) {
    const items = SERVICES.filter((s) => s.category === cat).sort(
      (a, b) => a.sort_order - b.sort_order
    );
    if (items.length > 0) {
      grouped[cat] = items;
    }
  }
  return grouped;
}

/** Formatea un precio en céntimos como texto (por ejemplo 2800 → "28€") */
export function formatPrice(cents: number): string {
  const euros = cents / 100;
  return euros % 1 === 0
    ? `${euros}€`
    : `${euros.toFixed(2).replace(".", ",")}€`;
}
