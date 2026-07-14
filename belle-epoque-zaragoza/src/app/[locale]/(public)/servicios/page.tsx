import { setRequestLocale } from "next-intl/server";
import OrnamentalDivider from "@/components/public/OrnamentalDivider";
import { Clock, Scissors, Sparkles, Zap, FlameKindling, Palette, Hand, Heart } from "lucide-react";
import Link from "next/link";
import { CATEGORY_ORDER, CATEGORY_LABELS, getGroupedServices, formatPrice } from "@/lib/services-data";
import type { ReactNode } from "react";

const CATEGORY_ICONS: Record<string, ReactNode> = {
  peluqueria:       <Scissors className="size-5" />,
  estetica_facial:  <Sparkles className="size-5" />,
  depilacion_laser: <Zap className="size-5" />,
  depilacion_cera:  <FlameKindling className="size-5" />,
  maquillaje:       <Palette className="size-5" />,
  manos_pies:       <Hand className="size-5" />,
  corporal:         <Heart className="size-5" />,
};

type Props = {
  params: Promise<{ locale: string }>;
};

export default async function ServicesPage({ params }: Props) {
  const { locale } = await params;
  setRequestLocale(locale);

  const grouped = getGroupedServices();

  return (
    <div className="min-h-screen bg-ivory pt-32 pb-24">
      <div className="max-w-5xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="text-center mb-20">
          <p className="label-caps mb-4 animate-[fadeIn_0.8s_ease-out]">
            Le Petit Palace
          </p>
          <h1 className="heading-display mb-6 animate-[fadeIn_0.8s_ease-out_0.2s_both]">
            Nuestros Servicios
          </h1>
          <OrnamentalDivider className="max-w-xs mx-auto animate-[fadeIn_0.8s_ease-out_0.4s_both]" />
          <p className="body-lg max-w-2xl mx-auto mt-6 animate-[fadeIn_0.8s_ease-out_0.5s_both]">
            Descubre nuestra carta completa de tratamientos de peluquería,
            estética y bienestar.
          </p>
        </div>

        <nav className="flex flex-wrap justify-center gap-2 mb-16 animate-[fadeIn_0.8s_ease-out_0.6s_both]">
          {CATEGORY_ORDER.map((cat) => (
            <a
              key={cat}
              href={`#${cat}`}
              className="inline-flex items-center gap-1.5 px-4 py-2 text-xs font-medium tracking-wide uppercase rounded-full border border-gold/20 text-charcoal/70 hover:border-gold hover:text-gold-dark hover:bg-gold/5 transition-all"
            >
              {CATEGORY_ICONS[cat]}
              {CATEGORY_LABELS[cat]}
            </a>
          ))}
        </nav>

        <div className="space-y-20">
          {CATEGORY_ORDER.map((cat, catIndex) => {
            const items = grouped[cat];
            if (!items) return null;

            return (
              <section
                key={cat}
                id={cat}
                className="scroll-mt-28 animate-[fadeIn_0.8s_ease-out_both]"
                style={{ animationDelay: `${0.7 + catIndex * 0.08}s` }}
              >
                <div className="flex items-center justify-center gap-3 mb-10">
                  <div className="flex-1 h-px bg-gradient-to-r from-transparent to-gold/30" />
                  <div className="flex items-center gap-2 text-gold-dark shrink-0">
                    {CATEGORY_ICONS[cat]}
                    <h2 className="heading-2 text-gold-dark whitespace-nowrap">
                      {CATEGORY_LABELS[cat]}
                    </h2>
                  </div>
                  <div className="flex-1 h-px bg-gradient-to-l from-transparent to-gold/30" />
                </div>

                <div className="bg-white rounded-2xl border border-gold/15 shadow-sm overflow-hidden">
                  <div className="divide-y divide-gold/10">
                    {items.map((service, idx) => (
                      <div
                        key={`${cat}-${idx}`}
                        className="group flex flex-col sm:flex-row sm:items-center justify-between gap-3 sm:gap-6 px-6 sm:px-10 py-5 hover:bg-ivory/60 transition-colors"
                      >
                        <div className="flex-1 min-w-0">
                          <h3 className="font-serif text-lg sm:text-xl font-medium text-charcoal group-hover:text-gold-dark transition-colors leading-snug">
                            {service.name}
                          </h3>
                          <p className="text-charcoal/55 text-sm leading-relaxed mt-1 line-clamp-2">
                            {service.description}
                          </p>
                        </div>

                        <div className="flex items-center gap-5 shrink-0">
                          {service.duration_minutes > 0 && (
                            <span className="flex items-center gap-1 text-xs text-charcoal/45 font-medium whitespace-nowrap">
                              <Clock className="size-3.5" />
                              {service.duration_minutes} min
                            </span>
                          )}
                          <span className="font-serif text-xl sm:text-2xl text-gold-dark tabular-nums min-w-[4.5rem] text-right">
                            {formatPrice(service.price_cents)}
                          </span>
                        </div>
                      </div>
                    ))}
                  </div>
                </div>
              </section>
            );
          })}
        </div>

        <div className="mt-24 text-center animate-[fadeIn_0.8s_ease-out_1.4s_both]">
          <p className="font-serif text-xl text-charcoal/60 mb-6 italic">
            ¿Ya has encontrado tu tratamiento?
          </p>
          <Link
            href={`/${locale}/reservar`}
            className="inline-flex items-center gap-2 px-10 py-4 bg-gold text-white text-lg font-medium rounded-full hover:bg-gold-dark transition-all hover:shadow-lg hover:shadow-gold/30 hover:-translate-y-0.5"
          >
            Reservar Cita
          </Link>
        </div>
      </div>
    </div>
  );
}
