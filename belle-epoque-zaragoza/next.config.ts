import type { NextConfig } from "next";
import createNextIntlPlugin from "next-intl/plugin";

const nextConfig: NextConfig = {
  images: {
    // Las imágenes se sirven desde /public/images. Desactivamos la
    // optimización para que las ilustraciones SVG se muestren tal cual
    // y para que el sitio funcione en cualquier hosting sin configuración.
    unoptimized: true,
  },
};

const withNextIntl = createNextIntlPlugin();

export default withNextIntl(nextConfig);
