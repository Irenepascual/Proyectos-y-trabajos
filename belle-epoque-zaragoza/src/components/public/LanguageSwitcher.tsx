"use client";

import { useLocale, useTranslations } from "next-intl";
import { useRouter, usePathname } from "next/navigation";
import { type Locale } from "@/i18n/routing";
import { Globe } from "lucide-react";

export default function LanguageSwitcher() {
  const locale = useLocale();
  const router = useRouter();
  const pathname = usePathname();
  const t = useTranslations("LanguageSwitcher");

  function handleChange(newLocale: Locale) {
    const newPath = pathname.replace(`/${locale}`, `/${newLocale}`);
    router.replace(newPath);
  }

  const otherLocale = locale === "es" ? "en" : "es";

  return (
    <button
      onClick={() => handleChange(otherLocale as Locale)}
      className="inline-flex items-center gap-1.5 px-3 py-1.5 text-sm font-medium text-charcoal/70 hover:text-gold border border-gold/20 rounded-full hover:border-gold/40 transition-colors"
      aria-label={t(otherLocale)}
    >
      <Globe className="size-3.5" />
      <span className="uppercase text-xs tracking-wider">
        {otherLocale.toUpperCase()}
      </span>
    </button>
  );
}
