import { setRequestLocale } from "next-intl/server";
import OrnamentalDivider from "@/components/public/OrnamentalDivider";
import BookingWizard from "@/components/public/BookingWizard";

type Props = {
  params: Promise<{ locale: string }>;
};

export default async function BookingPage({ params }: Props) {
  const { locale } = await params;
  setRequestLocale(locale);

  return (
    <div className="min-h-screen bg-ivory pt-28 pb-24">
      <div className="max-w-5xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="text-center mb-12">
          <p className="label-caps mb-4 animate-[fadeIn_0.8s_ease-out]">
            Le Petit Palace
          </p>
          <h1 className="heading-display mb-6 animate-[fadeIn_0.8s_ease-out_0.2s_both]">
            Reserva tu Cita
          </h1>
          <OrnamentalDivider className="max-w-xs mx-auto animate-[fadeIn_0.8s_ease-out_0.4s_both]" />
        </div>

        <BookingWizard />
      </div>
    </div>
  );
}
