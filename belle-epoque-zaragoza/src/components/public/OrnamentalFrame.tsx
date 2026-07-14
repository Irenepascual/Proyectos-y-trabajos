import { cn } from "@/lib/utils";
import OrnamentalCorner from "./OrnamentalCorner";

interface OrnamentalFrameProps {
  children: React.ReactNode;
  className?: string;
  cornerSize?: string;
  /** Mostrar solo las esquinas superiores (útil en tarjetas) */
  topOnly?: boolean;
}

/**
 * Marco decorativo con adornos barrocos en las esquinas.
 */
export default function OrnamentalFrame({
  children,
  className,
  cornerSize = "w-16 h-16 sm:w-20 sm:h-20",
  topOnly = false,
}: OrnamentalFrameProps) {
  return (
    <div className={cn("relative", className)}>
      <OrnamentalCorner className={cn("absolute top-0 left-0 -translate-x-1 -translate-y-1", cornerSize)} />
      <OrnamentalCorner className={cn("absolute top-0 right-0 translate-x-1 -translate-y-1 -scale-x-100", cornerSize)} />
      {!topOnly && (
        <>
          <OrnamentalCorner className={cn("absolute bottom-0 left-0 -translate-x-1 translate-y-1 -scale-y-100", cornerSize)} />
          <OrnamentalCorner className={cn("absolute bottom-0 right-0 translate-x-1 translate-y-1 -scale-x-100 -scale-y-100", cornerSize)} />
        </>
      )}
      {children}
    </div>
  );
}
