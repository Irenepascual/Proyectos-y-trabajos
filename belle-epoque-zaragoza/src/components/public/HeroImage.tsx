import { cn } from "@/lib/utils";
import Image from "next/image";
import OrnamentalFrame from "./OrnamentalFrame";

interface HeroImageProps {
  src: string;
  alt: string;
  className?: string;
  /** Mostrar el marco ornamental alrededor de la imagen */
  framed?: boolean;
  /** Aplicar viñeta dorada sobre la imagen */
  vignette?: boolean;
  /** Clase de proporción (aspect ratio) */
  aspect?: string;
  /** Carga prioritaria */
  priority?: boolean;
}

export default function HeroImage({
  src,
  alt,
  className,
  framed = true,
  vignette = true,
  aspect = "aspect-[3/4]",
  priority = false,
}: HeroImageProps) {
  const imageBlock = (
    <div className={cn("relative overflow-hidden rounded-sm", aspect, className)}>
      <Image
        src={src}
        alt={alt}
        fill
        className="object-cover"
        sizes="(max-width: 768px) 100vw, 50vw"
        priority={priority}
      />
      {vignette && (
        <div
          className="absolute inset-0 pointer-events-none"
          style={{
            background:
              "radial-gradient(ellipse at center, transparent 50%, rgba(168,135,62,0.08) 80%, rgba(43,43,43,0.15) 100%)",
          }}
        />
      )}
      <div className="absolute inset-x-0 bottom-0 h-1/4 bg-gradient-to-t from-charcoal/10 to-transparent pointer-events-none" />
    </div>
  );

  if (framed) {
    return (
      <OrnamentalFrame cornerSize="w-14 h-14 sm:w-18 sm:h-18 md:w-20 md:h-20">
        <div className="p-3 sm:p-4">{imageBlock}</div>
      </OrnamentalFrame>
    );
  }

  return imageBlock;
}
