import { useTranslations } from "next-intl";
import { setRequestLocale } from "next-intl/server";
import { use } from "react";
import Image from "next/image";
import { Phone, MapPin, Clock, AtSign } from "lucide-react";
import Link from "next/link";
import Navbar from "@/components/public/Navbar";
import Footer from "@/components/public/Footer";
import OrnamentalFrame from "@/components/public/OrnamentalFrame";
import OrnamentalDivider from "@/components/public/OrnamentalDivider";
import HeroImage from "@/components/public/HeroImage";

type Props = {
  params: Promise<{ locale: string }>;
};

export default function HomePage({ params }: Props) {
  const { locale } = use(params);
  setRequestLocale(locale);
  const t = useTranslations("Home");
  const footer = useTranslations("Footer");

  return (
    <>
      <Navbar />

      <main className="flex-1">
        {/* Hero */}
        <section className="relative min-h-screen flex items-center bg-ivory overflow-hidden pt-20">
          <div className="absolute inset-0 opacity-[0.03]" style={{ backgroundImage: "url(\"data:image/svg+xml,%3Csvg width='60' height='60' viewBox='0 0 60 60' xmlns='http://www.w3.org/2000/svg'%3E%3Cg fill='none' fill-rule='evenodd'%3E%3Cg fill='%23C9A961'%3E%3Cpath d='M36 34v-4h-2v4h-4v2h4v4h2v-4h4v-2h-4zm0-30V0h-2v4h-4v2h4v4h2V6h4V4h-4zM6 34v-4H4v4H0v2h4v4h2v-4h4v-2H6zM6 4V0H4v4H0v2h4v4h2V6h4V4H6z'/%3E%3C/g%3E%3C/g%3E%3C/svg%3E\")" }} />

          <div className="relative z-10 w-full max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
            <div className="flex flex-col lg:grid lg:grid-cols-12 gap-12 lg:gap-4 items-center min-h-[80vh]">
              <div className="w-full lg:col-span-6 lg:pr-8 text-center lg:text-left order-2 lg:order-1 mt-8 lg:mt-0">
                <p className="label-caps mb-4 animate-[fadeIn_0.8s_ease-out]">
                  Peluquería & Centro de Estética
                </p>

                <p className="text-script text-gold-dark mb-2 animate-[fadeIn_0.8s_ease-out_0.15s_both]">
                  {t("heroSubtitle")}
                </p>

                <h1 className="heading-display mb-6 animate-[fadeIn_0.8s_ease-out_0.3s_both] text-balance bg-gradient-to-r from-rose-accent via-gold to-gold-dark bg-clip-text text-transparent">
                  {t("heroTitle")}
                </h1>

                <OrnamentalDivider className="mb-6 animate-[fadeIn_0.8s_ease-out_0.4s_both] max-w-sm mx-auto lg:mx-0" />

                <p className="body-lg max-w-md mx-auto lg:mx-0 mb-10 animate-[fadeIn_0.8s_ease-out_0.5s_both]">
                  {t("heroDescription")}
                </p>

                <div className="flex flex-col sm:flex-row items-center justify-center lg:justify-start gap-4 animate-[fadeIn_0.8s_ease-out_0.6s_both]">
                  <Link
                    href={`/${locale}/reservar`}
                    className="inline-flex items-center gap-2 px-8 py-3.5 bg-gold text-white font-medium rounded-full hover:bg-gold-dark transition-all hover:shadow-lg hover:shadow-gold/20 hover:-translate-y-0.5"
                  >
                    {t("ctaBooking")}
                  </Link>
                  <Link
                    href={`/${locale}/servicios`}
                    className="inline-flex items-center gap-2 px-8 py-3.5 border border-gold/30 text-charcoal font-medium rounded-full hover:border-gold hover:bg-gold/5 transition-all"
                  >
                    {t("ctaServices")}
                  </Link>
                </div>
              </div>

              <div className="w-full lg:col-span-6 order-1 lg:order-2 flex justify-center lg:justify-end animate-[scaleIn_1s_ease-out_0.2s_both]">
                <div className="w-full max-w-md lg:max-w-none lg:w-[95%]">
                  <HeroImage
                    src="/images/hero.svg"
                    alt="Interior del salón Belle Epoque"
                    priority
                    aspect="aspect-[3/4] lg:aspect-[4/5]"
                  />
                </div>
              </div>
            </div>
          </div>
        </section>

        {/* Servicios destacados */}
        <section id="servicios" className="py-20 sm:py-28 bg-white">
          <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
            <div className="text-center mb-20">
              <p className="label-caps mb-3">{locale === "es" ? "Lo que nos hace únicos" : "What makes us unique"}</p>
              <h2 className="heading-1 mb-4">{t("featuredTitle")}</h2>
              <OrnamentalDivider className="max-w-xs mx-auto" />
            </div>

            <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6 lg:gap-8 max-w-7xl mx-auto">
              {[
                { id: "peeling", title: t("peelingTitle"), desc: t("peelingDescription"), img: "/images/servicio-peeling.svg" },
                { id: "laser", title: t("laserTitle"), desc: t("laserDescription"), img: "/images/servicio-laser.svg" },
                { id: "makeup", title: t("makeupTitle"), desc: t("makeupDescription"), img: "/images/servicio-maquillaje.svg" },
                { id: "hair", title: t("hairdressingTitle"), desc: t("hairdressingDescription"), img: "/images/servicio-peluqueria.svg" },
              ].map((service) => (
                <div key={service.id} className="group flex flex-col bg-ivory rounded-lg overflow-hidden border border-gold/10 hover:border-gold/30 transition-all hover:shadow-lg hover:shadow-gold/5">
                  <div className="relative w-full aspect-[4/3] overflow-hidden">
                    <Image
                      src={service.img}
                      alt={service.title}
                      fill
                      className="object-cover group-hover:scale-105 transition-transform duration-700"
                      sizes="(max-width: 768px) 100vw, 50vw"
                    />
                    <div className="absolute inset-0 bg-gradient-to-t from-charcoal/40 to-transparent opacity-60" />
                  </div>
                  <div className="p-6 flex flex-col flex-1">
                    <h3 className="heading-3 mb-2 group-hover:text-gold-dark transition-colors">{service.title}</h3>
                    <p className="text-charcoal/70 text-sm leading-relaxed mb-4 line-clamp-2 flex-1">
                      {service.desc}
                    </p>
                    <Link href={`/${locale}/servicios`} className="inline-flex items-center gap-2 text-gold-dark text-sm font-medium hover:text-gold transition-colors w-fit">
                      {locale === "es" ? "Saber más" : "Learn more"}
                      <span className="group-hover:translate-x-1 transition-transform">→</span>
                    </Link>
                  </div>
                </div>
              ))}
            </div>

            <div className="mt-16 text-center">
              <Link href={`/${locale}/servicios`} className="inline-flex items-center gap-2 px-8 py-3 border border-gold/30 text-charcoal font-medium rounded-full hover:border-gold hover:bg-gold/5 transition-all">
                {locale === "es" ? "Ver todos los servicios" : "View all services"}
              </Link>
            </div>
          </div>
        </section>

        {/* Horario y contacto */}
        <section id="horarios" className="py-20 sm:py-28 bg-ivory relative overflow-hidden">
          <div className="absolute -right-32 top-1/2 -translate-y-1/2 w-[500px] h-[500px] rounded-full bg-rose-blush/20 blur-3xl" />

          <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 relative z-10">
            <div className="grid grid-cols-1 lg:grid-cols-5 gap-12 lg:gap-16">
              <div className="lg:col-span-3">
                <OrnamentalFrame cornerSize="w-12 h-12 sm:w-16 sm:h-16">
                  <div className="bg-white p-8 sm:p-12">
                    <div className="flex items-center gap-3 mb-2">
                      <Clock className="size-4 text-gold" />
                      <p className="label-caps">{t("scheduleTitle")}</p>
                    </div>
                    <h2 className="heading-2 mb-8">{t("scheduleTitle")}</h2>

                    <div className="space-y-0">
                      {[
                        { day: t("scheduleMondayFriday"), hours: t("scheduleMondayFridayHours") },
                        { day: t("scheduleSaturday"), hours: t("scheduleSaturdayHours") },
                        { day: t("scheduleSunday"), hours: t("scheduleSundayHours") },
                      ].map((item, index) => (
                        <div key={index} className="flex items-center justify-between py-5 border-b border-gold/10 last:border-b-0">
                          <span className="text-charcoal font-medium text-lg">{item.day}</span>
                          <span className="text-gold-dark font-serif text-xl tracking-wide">{item.hours}</span>
                        </div>
                      ))}
                    </div>
                  </div>
                </OrnamentalFrame>
              </div>

              <div className="lg:col-span-2 flex flex-col justify-center">
                <div className="flex items-center gap-3 mb-2">
                  <MapPin className="size-4 text-gold" />
                  <p className="label-caps">{t("locationTitle")}</p>
                </div>
                <h2 className="heading-3 mb-6">{t("address")}</h2>

                <div className="space-y-4">
                  <a
                    href="tel:876652076"
                    className="flex items-center gap-3 px-5 py-3.5 bg-white border border-gold/15 rounded-lg hover:border-gold/40 transition-all group"
                  >
                    <Phone className="size-4 text-gold group-hover:text-gold-dark transition-colors" />
                    <div>
                      <p className="text-xs text-charcoal/50 font-medium">{footer("phone")}</p>
                      <p className="text-charcoal font-medium">876 65 20 76</p>
                    </div>
                  </a>
                  <a
                    href="https://wa.me/34649810095"
                    target="_blank"
                    rel="noopener noreferrer"
                    className="flex items-center gap-3 px-5 py-3.5 bg-white border border-gold/15 rounded-lg hover:border-gold/40 transition-all group"
                  >
                    <Phone className="size-4 text-gold group-hover:text-gold-dark transition-colors" />
                    <div>
                      <p className="text-xs text-charcoal/50 font-medium">WhatsApp</p>
                      <p className="text-charcoal font-medium">649 81 00 95</p>
                    </div>
                  </a>
                  <a
                    href="https://www.instagram.com/belleepoquezaragoza"
                    target="_blank"
                    rel="noopener noreferrer"
                    className="flex items-center gap-3 px-5 py-3.5 bg-white border border-gold/15 rounded-lg hover:border-gold/40 transition-all group"
                  >
                    <AtSign className="size-4 text-gold group-hover:text-gold-dark transition-colors" />
                    <div>
                      <p className="text-xs text-charcoal/50 font-medium">Instagram</p>
                      <p className="text-charcoal font-medium">@belleepoquezaragoza</p>
                    </div>
                  </a>
                </div>
              </div>
            </div>
          </div>
        </section>

        {/* Llamada a la acción */}
        <section id="reservar" className="relative py-28 sm:py-36 overflow-hidden">
          <Image
            src="/images/cta.svg"
            alt=""
            fill
            className="object-cover"
            aria-hidden="true"
          />
          <div className="absolute inset-0 bg-charcoal/75" />
          <div className="absolute inset-0 bg-gradient-to-r from-charcoal/90 via-charcoal/70 to-charcoal/90" />

          <div className="relative z-10 max-w-3xl mx-auto text-center px-4">
            <p className="font-script text-3xl sm:text-4xl text-gold mb-2">
              Le Petit Palace
            </p>
            <h2 className="font-serif text-3xl sm:text-4xl md:text-5xl font-light text-white mb-4 tracking-wide">
              {t("ctaBooking")}
            </h2>
            <OrnamentalDivider className="max-w-xs mx-auto mb-8 [&_*]:!text-gold/60 [&>div]:!from-transparent [&>div]:!via-gold/20 [&>div]:!to-gold/40" />
            <Link
              href={`/${locale}/reservar`}
              className="inline-flex items-center gap-2 px-10 py-4 bg-gold text-white text-lg font-medium rounded-full hover:bg-gold-dark transition-all hover:shadow-lg hover:shadow-gold/30 hover:-translate-y-0.5"
            >
              {t("ctaBooking")}
            </Link>
          </div>
        </section>
      </main>

      <Footer />
    </>
  );
}
