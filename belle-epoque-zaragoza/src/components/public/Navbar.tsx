"use client";

import { useState } from "react";
import Link from "next/link";
import { useLocale, useTranslations } from "next-intl";
import LanguageSwitcher from "./LanguageSwitcher";
import { Menu, X } from "lucide-react";

export default function Navbar() {
  const nav = useTranslations("Navigation");
  const locale = useLocale();
  const [isOpen, setIsOpen] = useState(false);

  const toggleMenu = () => setIsOpen(!isOpen);
  const closeMenu = () => setIsOpen(false);

  return (
    <header className="fixed top-0 left-0 right-0 z-50 bg-ivory/95 backdrop-blur-md border-b border-gold/10">
      <nav className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 flex items-center justify-between h-16 sm:h-20">
        <Link href={`/${locale}`} className="font-script text-2xl sm:text-3xl text-gold-dark z-50" onClick={closeMenu}>
          Le Petit Palace
        </Link>

        {/* Menú de escritorio */}
        <div className="hidden md:flex items-center gap-8">
          <Link href={`/${locale}`} className="label-caps !text-charcoal hover:!text-gold transition-colors">{nav("home")}</Link>
          <Link href={`/${locale}/servicios`} className="label-caps !text-charcoal hover:!text-gold transition-colors">{nav("services")}</Link>
          <Link href={`/${locale}/galeria`} className="label-caps !text-charcoal hover:!text-gold transition-colors">{nav("gallery")}</Link>
          <a href={`/${locale}#horarios`} className="label-caps !text-charcoal hover:!text-gold transition-colors">{nav("contact")}</a>
          <Link href={`/${locale}/reservar`} className="inline-flex items-center gap-2 px-6 py-2.5 bg-gold text-white text-sm font-medium rounded-full hover:bg-gold-dark transition-all hover:shadow-lg hover:shadow-gold/20">
            {nav("booking")}
          </Link>
          <LanguageSwitcher />
        </div>

        {/* Menú móvil */}
        <div className="flex md:hidden items-center gap-4 z-50">
          <LanguageSwitcher />
          <button onClick={toggleMenu} className="p-2 text-charcoal hover:text-gold transition-colors" aria-label="Menú">
            {isOpen ? <X className="size-6" /> : <Menu className="size-6" />}
          </button>
        </div>
      </nav>

      {/* Menú móvil desplegado */}
      {isOpen && (
        <div className="md:hidden fixed inset-0 top-[64px] sm:top-[80px] bg-ivory z-40 border-t border-gold/10 flex flex-col p-6 animate-[fadeIn_0.2s_ease-out]">
          <Link href={`/${locale}`} onClick={closeMenu} className="py-4 border-b border-gold/10 label-caps !text-charcoal hover:!text-gold text-lg">
            {nav("home")}
          </Link>
          <Link href={`/${locale}/servicios`} onClick={closeMenu} className="py-4 border-b border-gold/10 label-caps !text-charcoal hover:!text-gold text-lg">
            {nav("services")}
          </Link>
          <Link href={`/${locale}/galeria`} onClick={closeMenu} className="py-4 border-b border-gold/10 label-caps !text-charcoal hover:!text-gold text-lg">
            {nav("gallery")}
          </Link>
          <a href={`/${locale}#horarios`} onClick={closeMenu} className="py-4 border-b border-gold/10 label-caps !text-charcoal hover:!text-gold text-lg">
            {nav("contact")}
          </a>
          <div className="mt-8">
            <Link href={`/${locale}/reservar`} onClick={closeMenu} className="w-full inline-flex justify-center items-center gap-2 px-6 py-4 bg-gold text-white text-sm font-medium rounded-full hover:bg-gold-dark transition-all">
              {nav("booking")}
            </Link>
          </div>
        </div>
      )}
    </header>
  );
}
