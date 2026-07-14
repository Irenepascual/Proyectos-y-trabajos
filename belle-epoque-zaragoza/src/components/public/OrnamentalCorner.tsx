import { cn } from "@/lib/utils";

interface OrnamentalCornerProps {
  className?: string;
}

/**
 * Adorno de esquina de inspiración barroca.
 * Por defecto se dibuja como esquina superior izquierda; usa className para rotarlo.
 */
export default function OrnamentalCorner({ className }: OrnamentalCornerProps) {
  return (
    <svg
      viewBox="0 0 100 100"
      fill="none"
      xmlns="http://www.w3.org/2000/svg"
      className={cn("text-gold", className)}
      aria-hidden="true"
    >
      <path d="M 0 100 L 0 20 C 0 9 9 0 20 0 L 100 0" stroke="currentColor" strokeWidth="2" />
      <path d="M 6 94 L 6 23 C 6 13 13 6 23 6 L 94 6" stroke="currentColor" strokeWidth="0.75" opacity="0.4" />
      <path d="M 0 55 C 8 45 14 36 20 28 C 26 20 34 14 45 10 C 52 7 55 0 55 0" stroke="currentColor" strokeWidth="1.2" strokeLinecap="round" />
      <path d="M 0 42 C 10 34 18 27 25 25 C 27 18 34 10 42 0" stroke="currentColor" strokeWidth="0.8" opacity="0.5" strokeLinecap="round" />
      <path d="M 22 22 C 26 17 27 13 25 9 C 21 12 17 17 17 22 C 17 27 12 28 9 25" stroke="currentColor" strokeWidth="0.7" fill="none" opacity="0.45" />
      <circle cx="20" cy="20" r="2" fill="currentColor" opacity="0.3" />
      <path d="M 68 1.5 Q 71 -1 74 1.5 Q 71 4 68 1.5Z" fill="currentColor" opacity="0.2" />
      <path d="M 1.5 68 Q -1 71 1.5 74 Q 4 71 1.5 68Z" fill="currentColor" opacity="0.2" />
    </svg>
  );
}
