"use client";

import { useState } from "react";
import Image from "next/image";
import Lightbox from "yet-another-react-lightbox";
import "yet-another-react-lightbox/styles.css";
import { cn } from "@/lib/utils";
import type { GalleryItem } from "@/lib/gallery-data";

interface GalleryGridProps {
  items: GalleryItem[];
}

export default function GalleryGrid({ items }: GalleryGridProps) {
  const [activeCategory, setActiveCategory] = useState<string>("Todos");
  const [lightboxOpen, setLightboxOpen] = useState(false);
  const [currentIndex, setCurrentIndex] = useState(0);

  const categories = ["Todos", ...Array.from(new Set(items.map((item) => item.category)))];

  const filteredItems = items.filter(
    (item) => activeCategory === "Todos" || item.category === activeCategory
  );

  const slides = filteredItems.map((item) => ({
    src: item.image,
    alt: item.description || item.category,
    description: item.description || item.category,
  }));

  const handleImageClick = (index: number) => {
    setCurrentIndex(index);
    setLightboxOpen(true);
  };

  if (items.length === 0) {
    return (
      <div className="text-center py-20 animate-[fadeIn_0.8s_ease-out_0.6s_both]">
        <p className="font-serif text-2xl text-charcoal/50 italic mb-4">
          Próximamente...
        </p>
        <p className="text-charcoal/70">
          Nuestra galería estará disponible muy pronto.
        </p>
      </div>
    );
  }

  return (
    <>
      {/* Filtros por categoría */}
      <div className="flex flex-wrap justify-center gap-2 sm:gap-4 mb-12 animate-[fadeIn_0.8s_ease-out_0.6s_both]">
        {categories.map((category) => (
          <button
            key={category}
            onClick={() => setActiveCategory(category)}
            className={cn(
              "px-5 py-2 rounded-full text-sm font-medium transition-all",
              activeCategory === category
                ? "bg-gold text-white shadow-md shadow-gold/20"
                : "bg-white text-charcoal/70 border border-gold/15 hover:border-gold/40 hover:text-gold-dark"
            )}
          >
            {category}
          </button>
        ))}
      </div>

      {/* Cuadrícula */}
      <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-4 animate-[fadeIn_0.8s_ease-out_0.8s_both]">
        {filteredItems.map((item, index) => (
          <div
            key={item.id}
            className="group relative aspect-square overflow-hidden rounded-xl cursor-pointer bg-muted"
            onClick={() => handleImageClick(index)}
          >
            <Image
              src={item.image}
              alt={item.description || item.category}
              fill
              className="object-cover group-hover:scale-105 transition-transform duration-700"
              sizes="(max-width: 640px) 100vw, (max-width: 768px) 50vw, (max-width: 1024px) 33vw, 25vw"
            />
            <div className="absolute inset-0 bg-charcoal/0 group-hover:bg-charcoal/40 transition-colors duration-300 flex items-center justify-center">
              <span className="opacity-0 group-hover:opacity-100 text-white font-medium tracking-wide translate-y-4 group-hover:translate-y-0 transition-all duration-300">
                Ver imagen
              </span>
            </div>
          </div>
        ))}
      </div>

      {/* Visor a pantalla completa */}
      <Lightbox
        open={lightboxOpen}
        close={() => setLightboxOpen(false)}
        index={currentIndex}
        slides={slides}
        carousel={{ finite: slides.length <= 1 }}
        render={{
          slide: ({ slide }) => {
            const s = slide as { src: string; alt?: string; description?: string };
            return (
              <div className="relative w-full h-full flex flex-col items-center justify-center p-4">
                <div className="relative max-w-[90vw] max-h-[85vh] w-auto h-auto aspect-auto">
                  {/* eslint-disable-next-line @next/next/no-img-element */}
                  <img
                    src={s.src}
                    alt={s.alt || ""}
                    className="max-w-full max-h-[85vh] object-contain rounded-sm"
                  />
                </div>
                {s.description && (
                  <p className="text-white/80 mt-4 text-center font-serif text-lg tracking-wide">
                    {s.description}
                  </p>
                )}
              </div>
            );
          },
        }}
      />
    </>
  );
}
