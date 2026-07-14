import { useTranslations } from "next-intl";

export default function Footer() {
  const footer = useTranslations("Footer");

  return (
    <footer className="bg-charcoal text-white/60 py-12 border-t border-white/5">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="flex flex-col md:flex-row items-center justify-between gap-6">
          <div className="text-center md:text-left">
            <p className="font-script text-2xl text-gold mb-1">Le Petit Palace</p>
            <p className="text-sm">
              © {new Date().getFullYear()} Belle Epoque Zaragoza. {footer("rights")}
            </p>
          </div>
          <div className="flex items-center gap-6 text-sm">
            <a href="#" className="hover:text-gold transition-colors">{footer("legalNotice")}</a>
            <a href="#" className="hover:text-gold transition-colors">{footer("privacyPolicy")}</a>
            <a href="#" className="hover:text-gold transition-colors">{footer("cookiePolicy")}</a>
          </div>
        </div>
      </div>
    </footer>
  );
}
