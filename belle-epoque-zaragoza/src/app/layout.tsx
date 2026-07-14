import type { Metadata } from "next";

export const metadata: Metadata = {
  title: "Belle Epoque Zaragoza — Le Petit Palace",
  description:
    "Peluquería y centro de estética de alta gama en Zaragoza. Servicios de peluquería, maquillaje profesional, depilación láser y tratamientos faciales.",
};

// El layout raíz es intencionadamente mínimo.
// El layout de [locale] aporta <html>, <body>, las fuentes y el idioma.
export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return children;
}
