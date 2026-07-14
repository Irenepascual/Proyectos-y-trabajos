import { routing } from "@/i18n/routing";
import { redirect } from "next/navigation";

// La raíz redirige al idioma por defecto (español).
export default function RootPage() {
  redirect(`/${routing.defaultLocale}`);
}
