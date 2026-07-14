import { cn } from "@/lib/utils";

interface OrnamentalDividerProps {
  className?: string;
}

/**
 * Separador decorativo horizontal con un motivo central
 * y líneas doradas que se extienden a los lados.
 */
export default function OrnamentalDivider({ className }: OrnamentalDividerProps) {
  return (
    <div
      className={cn("flex items-center justify-center gap-0 w-full py-4", className)}
      aria-hidden="true"
    >
      <div className="flex-1 h-px bg-gradient-to-r from-transparent via-gold/30 to-gold/60" />

      <svg
        viewBox="0 0 120 40"
        fill="none"
        xmlns="http://www.w3.org/2000/svg"
        className="w-28 sm:w-36 h-auto text-gold shrink-0 mx-1"
      >
        <path d="M 5 20 C 10 10 20 8 30 12 C 35 14 38 17 40 20 C 38 23 35 26 30 28 C 20 32 10 30 5 20Z" stroke="currentColor" strokeWidth="0.8" fill="none" opacity="0.5" />
        <path d="M 15 20 C 18 15 25 14 30 17" stroke="currentColor" strokeWidth="0.6" fill="none" opacity="0.35" />
        <path d="M 60 8 L 68 20 L 60 32 L 52 20 Z" stroke="currentColor" strokeWidth="1" fill="currentColor" fillOpacity="0.08" />
        <circle cx="60" cy="20" r="4" stroke="currentColor" strokeWidth="0.8" fill="currentColor" fillOpacity="0.1" />
        <circle cx="60" cy="20" r="1.5" fill="currentColor" opacity="0.4" />
        <path d="M 115 20 C 110 10 100 8 90 12 C 85 14 82 17 80 20 C 82 23 85 26 90 28 C 100 32 110 30 115 20Z" stroke="currentColor" strokeWidth="0.8" fill="none" opacity="0.5" />
        <path d="M 105 20 C 102 15 95 14 90 17" stroke="currentColor" strokeWidth="0.6" fill="none" opacity="0.35" />
        <line x1="40" y1="20" x2="52" y2="20" stroke="currentColor" strokeWidth="0.5" opacity="0.4" />
        <line x1="68" y1="20" x2="80" y2="20" stroke="currentColor" strokeWidth="0.5" opacity="0.4" />
      </svg>

      <div className="flex-1 h-px bg-gradient-to-l from-transparent via-gold/30 to-gold/60" />
    </div>
  );
}
