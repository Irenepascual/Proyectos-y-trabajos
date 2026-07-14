"use client";

import { useState, useMemo, useCallback } from "react";
import { useLocale } from "next-intl";
import { cn } from "@/lib/utils";
import {
  SERVICES,
  CATEGORY_ORDER,
  CATEGORY_LABELS,
  getGroupedServices,
  formatPrice,
} from "@/lib/services-data";
import { ChevronLeft, ChevronRight, Check, Calendar, ArrowRight } from "lucide-react";

/* ─── Horario del salón ─── */
const BUSINESS_HOURS: Record<number, { open: string; close: string } | null> = {
  0: null,                          // Domingo — cerrado
  1: { open: "09:30", close: "20:00" },
  2: { open: "09:30", close: "20:00" },
  3: { open: "09:30", close: "20:00" },
  4: { open: "09:30", close: "20:00" },
  5: { open: "09:30", close: "20:00" },
  6: { open: "09:30", close: "14:00" },
};
const SLOT_INTERVAL = 15;  // minutos entre horas disponibles
const BUFFER_MINUTES = 10; // margen entre citas

/* ─── Helpers ─── */
const MONTHS_ES = ["Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"];
const DAYS_SHORT = ["L","M","X","J","V","S","D"];

function toDateKey(d: Date) {
  return `${d.getFullYear()}-${String(d.getMonth()+1).padStart(2,"0")}-${String(d.getDate()).padStart(2,"0")}`;
}

function parseTime(time: string, refDate: Date): Date {
  const [h, m] = time.split(":").map(Number);
  const d = new Date(refDate);
  d.setHours(h, m, 0, 0);
  return d;
}

function addMin(d: Date, mins: number): Date {
  return new Date(d.getTime() + mins * 60000);
}

function generateSlots(date: Date, totalDuration: number): string[] {
  const dow = date.getDay();
  const hours = BUSINESS_HOURS[dow];
  if (!hours) return [];

  const open = parseTime(hours.open, date);
  const close = parseTime(hours.close, date);
  const now = new Date();
  const slots: string[] = [];
  let cursor = new Date(open);

  while (true) {
    const end = addMin(cursor, totalDuration + BUFFER_MINUTES);
    if (end > close) break;
    if (cursor > now) {
      slots.push(`${String(cursor.getHours()).padStart(2,"0")}:${String(cursor.getMinutes()).padStart(2,"0")}`);
    }
    cursor = addMin(cursor, SLOT_INTERVAL);
  }
  return slots;
}

function isDayAvailable(date: Date): boolean {
  const dow = date.getDay();
  return BUSINESS_HOURS[dow] !== null;
}

/* ─── Calendario ─── */
function MiniCalendar({
  year, month, selectedKey, onSelect, maxDate,
  onPrev, onNext, canPrev, canNext,
}: {
  year: number; month: number; selectedKey: string | null;
  onSelect: (d: Date) => void; maxDate: Date;
  onPrev: () => void; onNext: () => void;
  canPrev: boolean; canNext: boolean;
}) {
  const firstDay = new Date(year, month, 1);
  const daysInMonth = new Date(year, month + 1, 0).getDate();
  // Lunes = primera columna
  let startOffset = firstDay.getDay() - 1;
  if (startOffset < 0) startOffset = 6;
  const today = new Date(); today.setHours(0,0,0,0);

  const cells: (number | null)[] = [];
  for (let i = 0; i < startOffset; i++) cells.push(null);
  for (let d = 1; d <= daysInMonth; d++) cells.push(d);

  return (
    <div>
      <div className="flex items-center justify-between mb-5">
        <button onClick={onPrev} disabled={!canPrev}
          className={cn("p-2 rounded-full transition-colors", canPrev ? "hover:bg-gold/10 text-charcoal" : "text-charcoal/20 cursor-not-allowed")}>
          <ChevronLeft className="size-5" />
        </button>
        <h3 className="font-serif text-xl text-charcoal tracking-wide">
          {MONTHS_ES[month]} {year}
        </h3>
        <button onClick={onNext} disabled={!canNext}
          className={cn("p-2 rounded-full transition-colors", canNext ? "hover:bg-gold/10 text-charcoal" : "text-charcoal/20 cursor-not-allowed")}>
          <ChevronRight className="size-5" />
        </button>
      </div>
      <div className="grid grid-cols-7 mb-2">
        {DAYS_SHORT.map((d) => (
          <div key={d} className="text-center text-xs font-medium text-charcoal/40 py-1">{d}</div>
        ))}
      </div>
      <div className="grid grid-cols-7 gap-y-1">
        {cells.map((day, i) => {
          if (day === null) return <div key={`e-${i}`} />;
          const date = new Date(year, month, day);
          date.setHours(0,0,0,0);
          const key = toDateKey(date);
          const isPast = date < today;
          const isFuture = date > maxDate;
          const available = !isPast && !isFuture && isDayAvailable(date);
          const selected = selectedKey === key;

          return (
            <button key={key} disabled={!available}
              onClick={() => onSelect(date)}
              className={cn(
                "aspect-square flex items-center justify-center text-sm rounded-full mx-auto w-10 h-10 transition-all",
                selected
                  ? "bg-gold text-white font-semibold shadow-md shadow-gold/25"
                  : available
                    ? "text-charcoal hover:bg-gold/10 hover:text-gold-dark cursor-pointer"
                    : "text-charcoal/20 cursor-not-allowed"
              )}>
              {day}
            </button>
          );
        })}
      </div>
    </div>
  );
}

/* ─── Indicador de pasos ─── */
const STEPS = [
  { num: 1, label: "Servicios" },
  { num: 2, label: "Fecha" },
  { num: 3, label: "Hora" },
  { num: 4, label: "Datos" },
];

function StepIndicator({ current }: { current: number }) {
  return (
    <div className="flex items-center justify-center gap-0 mb-10">
      {STEPS.map((s, i) => (
        <div key={s.num} className="flex items-center">
          <div className="flex flex-col items-center">
            <div className={cn(
              "w-9 h-9 rounded-full flex items-center justify-center text-sm font-medium transition-all border-2",
              current === s.num
                ? "border-gold bg-gold text-white"
                : current > s.num
                  ? "border-gold/40 bg-gold/10 text-gold-dark"
                  : "border-charcoal/15 bg-white text-charcoal/30"
            )}>
              {current > s.num ? <Check className="size-4" /> : s.num}
            </div>
            <span className={cn(
              "text-[10px] sm:text-xs mt-1.5 uppercase tracking-widest font-medium whitespace-nowrap",
              current >= s.num ? "text-charcoal/70" : "text-charcoal/25"
            )}>{s.label}</span>
          </div>
          {i < STEPS.length - 1 && (
            <div className={cn(
              "w-8 sm:w-16 h-px mx-1 sm:mx-2 mb-5 transition-colors",
              current > s.num ? "bg-gold/40" : "bg-charcoal/10"
            )} />
          )}
        </div>
      ))}
    </div>
  );
}

export default function BookingWizard() {
  const locale = useLocale();
  const grouped = useMemo(() => getGroupedServices(), []);

  const [step, setStep] = useState(1);
  const [selectedIds, setSelectedIds] = useState<string[]>([]);
  const [calMonth, setCalMonth] = useState(() => { const n = new Date(); return { y: n.getFullYear(), m: n.getMonth() }; });
  const [selectedDate, setSelectedDate] = useState<Date | null>(null);
  const [selectedTime, setSelectedTime] = useState<string | null>(null);
  const [form, setForm] = useState({ name: "", lastName: "", phone: "", email: "", notes: "" });
  const [wantsReminder, setWantsReminder] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [submitted, setSubmitted] = useState(false);

  const selectedServices = useMemo(() => SERVICES.filter((s) => selectedIds.includes(s.id)), [selectedIds]);
  const totalPrice = useMemo(() => selectedServices.reduce((a, s) => a + s.price_cents, 0), [selectedServices]);
  const totalDuration = useMemo(() => selectedServices.reduce((a, s) => a + s.duration_minutes, 0), [selectedServices]);

  const maxDate = useMemo(() => { const d = new Date(); d.setFullYear(d.getFullYear() + 1); return d; }, []);
  const today = useMemo(() => { const d = new Date(); d.setHours(0,0,0,0); return d; }, []);

  const slots = useMemo(() => {
    if (!selectedDate || totalDuration === 0) return [];
    return generateSlots(selectedDate, totalDuration);
  }, [selectedDate, totalDuration]);

  const canPrev = calMonth.y > today.getFullYear() || (calMonth.y === today.getFullYear() && calMonth.m > today.getMonth());
  const canNext = calMonth.y < maxDate.getFullYear() || (calMonth.y === maxDate.getFullYear() && calMonth.m < maxDate.getMonth());

  const toggleService = useCallback((id: string) => {
    setSelectedIds((prev) => prev.includes(id) ? prev.filter((x) => x !== id) : [...prev, id]);
  }, []);

  const goNext = () => {
    setError(null);
    if (step === 1 && selectedIds.length === 0) { setError("Selecciona al menos un servicio."); return; }
    if (step === 2 && !selectedDate) { setError("Selecciona un día."); return; }
    if (step === 3 && !selectedTime) { setError("Selecciona una hora."); return; }
    setStep((s) => s + 1);
  };
  const goBack = () => { setError(null); setStep((s) => s - 1); };

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    if (!form.name.trim() || !form.lastName.trim() || !form.phone.trim()) {
      setError("Rellena los campos obligatorios."); return;
    }
    if (form.phone.replace(/\s/g, "").length < 9) {
      setError("El teléfono no parece válido."); return;
    }
    setSubmitted(true);
  };

  if (submitted) {
    const dateStr = selectedDate
      ? `${selectedDate.getDate()} de ${MONTHS_ES[selectedDate.getMonth()]} de ${selectedDate.getFullYear()}`
      : "";
    return (
      <div className="max-w-2xl mx-auto animate-[fadeIn_0.5s_ease-out]">
        <div className="bg-white rounded-2xl p-8 sm:p-12 text-center shadow-sm border border-gold/15">
          <div className="w-16 h-16 rounded-full bg-gold/10 flex items-center justify-center mx-auto mb-6">
            <Check className="size-8 text-gold" />
          </div>
          <h2 className="heading-2 mb-3">Solicitud enviada</h2>
          <p className="text-charcoal/60 mb-8 max-w-md mx-auto">
            Gracias por confiar en Belle Epoque. Nos pondremos en contacto contigo para confirmar tu cita.
          </p>
          <div className="bg-ivory rounded-xl p-6 text-left space-y-3 mb-8 border border-gold/10">
            <div className="flex justify-between">
              <span className="text-charcoal/50 text-sm">Fecha</span>
              <span className="font-medium text-charcoal">{dateStr} a las {selectedTime}</span>
            </div>
            <div className="h-px bg-gold/10" />
            <div className="flex justify-between">
              <span className="text-charcoal/50 text-sm">Servicios</span>
              <span className="font-medium text-charcoal text-right max-w-[60%]">{selectedServices.map((s) => s.name).join(", ")}</span>
            </div>
            <div className="h-px bg-gold/10" />
            <div className="flex justify-between">
              <span className="text-charcoal/50 text-sm">Total</span>
              <span className="font-serif text-xl text-gold-dark">{formatPrice(totalPrice)}</span>
            </div>
          </div>
          <p className="text-xs text-charcoal/40 mb-6">
            Para cualquier cambio, llámanos al 876 65 20 76
          </p>
          <a href={`/${locale}`}
            className="inline-flex items-center gap-2 px-8 py-3 bg-gold text-white font-medium rounded-full hover:bg-gold-dark transition-all">
            Volver al inicio
          </a>
        </div>
      </div>
    );
  }

  return (
    <div className="max-w-4xl mx-auto animate-[fadeIn_0.4s_ease-out]">
      <StepIndicator current={step} />

      <div className="bg-white rounded-2xl shadow-sm border border-gold/15 overflow-hidden">
        {error && (
          <div className="mx-6 mt-6 p-4 rounded-lg bg-rose-blush/40 text-charcoal/80 text-sm border border-rose-accent/30">
            {error}
          </div>
        )}

        <div className="p-6 sm:p-10 min-h-[480px] flex flex-col">
          {/* Paso 1: Servicios */}
          {step === 1 && (
            <div className="flex-1 animate-[fadeIn_0.3s_ease-out]">
              <h2 className="font-serif text-2xl text-charcoal text-center mb-8">Elige tus servicios</h2>
              <div className="space-y-8">
                {CATEGORY_ORDER.map((cat) => {
                  const items = grouped[cat];
                  if (!items) return null;
                  return (
                    <div key={cat}>
                      <h3 className="font-serif text-lg text-gold-dark mb-3 pb-2 border-b border-gold/10">
                        {CATEGORY_LABELS[cat]}
                      </h3>
                      <div className="grid grid-cols-1 sm:grid-cols-2 gap-3">
                        {items.map((service) => {
                          const on = selectedIds.includes(service.id);
                          return (
                            <button key={service.id} type="button"
                              onClick={() => toggleService(service.id)}
                              className={cn(
                                "text-left p-4 rounded-xl border transition-all",
                                on ? "border-gold bg-gold/5 shadow-sm" : "border-charcoal/10 hover:border-gold/40"
                              )}>
                              <div className="flex items-start justify-between gap-2">
                                <span className="font-medium text-charcoal text-sm leading-snug">{service.name}</span>
                                <div className={cn(
                                  "w-5 h-5 rounded-full border-2 flex items-center justify-center shrink-0 mt-0.5 transition-colors",
                                  on ? "border-gold bg-gold" : "border-charcoal/20"
                                )}>
                                  {on && <Check className="size-3 text-white" />}
                                </div>
                              </div>
                              <span className="text-gold-dark font-serif text-base mt-1 block">
                                {formatPrice(service.price_cents)}
                              </span>
                            </button>
                          );
                        })}
                      </div>
                    </div>
                  );
                })}
              </div>
            </div>
          )}

          {/* Paso 2: Fecha */}
          {step === 2 && (
            <div className="flex-1 animate-[fadeIn_0.3s_ease-out]">
              <h2 className="font-serif text-2xl text-charcoal text-center mb-2">Elige el día</h2>
              <p className="text-center text-charcoal/50 text-sm mb-8">
                Puedes reservar hasta con un año de antelación.
              </p>
              <div className="max-w-sm mx-auto">
                <MiniCalendar
                  year={calMonth.y} month={calMonth.m}
                  selectedKey={selectedDate ? toDateKey(selectedDate) : null}
                  onSelect={(d) => { setSelectedDate(d); setSelectedTime(null); }}
                  maxDate={maxDate}
                  onPrev={() => setCalMonth((p) => p.m === 0 ? { y: p.y - 1, m: 11 } : { y: p.y, m: p.m - 1 })}
                  onNext={() => setCalMonth((p) => p.m === 11 ? { y: p.y + 1, m: 0 } : { y: p.y, m: p.m + 1 })}
                  canPrev={canPrev} canNext={canNext}
                />
                {selectedDate && (
                  <div className="mt-6 text-center">
                    <p className="text-sm text-charcoal/60">
                      Día seleccionado: <span className="font-medium text-charcoal">
                        {selectedDate.getDate()} de {MONTHS_ES[selectedDate.getMonth()]}
                      </span>
                    </p>
                  </div>
                )}
              </div>
            </div>
          )}

          {/* Paso 3: Hora */}
          {step === 3 && (
            <div className="flex-1 animate-[fadeIn_0.3s_ease-out]">
              <h2 className="font-serif text-2xl text-charcoal text-center mb-2">Elige la hora</h2>
              <p className="text-center text-charcoal/50 text-sm mb-8">
                {selectedDate && (
                  <>{selectedDate.getDate()} de {MONTHS_ES[selectedDate.getMonth()]} de {selectedDate.getFullYear()}</>
                )}
              </p>
              {slots.length === 0 ? (
                <div className="flex flex-col items-center justify-center py-16 text-center">
                  <Calendar className="size-10 text-charcoal/20 mb-4" />
                  <p className="text-charcoal/50">No hay huecos disponibles este día.</p>
                  <button onClick={goBack}
                    className="mt-4 text-gold-dark text-sm font-medium hover:underline">
                    Elegir otro día
                  </button>
                </div>
              ) : (
                <div className="grid grid-cols-3 sm:grid-cols-4 md:grid-cols-5 gap-3 max-w-lg mx-auto">
                  {slots.map((time) => (
                    <button key={time} type="button"
                      onClick={() => setSelectedTime(time)}
                      className={cn(
                        "py-3 px-2 rounded-lg border text-sm font-medium transition-all text-center",
                        selectedTime === time
                          ? "border-gold bg-gold text-white shadow-md shadow-gold/20"
                          : "border-charcoal/10 text-charcoal hover:border-gold/50 hover:bg-gold/5"
                      )}>
                      {time}
                    </button>
                  ))}
                </div>
              )}
            </div>
          )}

          {/* Paso 4: Datos del cliente */}
          {step === 4 && (
            <form id="booking-form" onSubmit={handleSubmit} className="flex-1 animate-[fadeIn_0.3s_ease-out]">
              <h2 className="font-serif text-2xl text-charcoal text-center mb-8">Tus datos</h2>

              <div className="bg-ivory rounded-xl p-4 mb-8 flex flex-wrap items-center justify-between gap-3 text-sm border border-gold/10">
                <div>
                  <span className="text-charcoal/50">Fecha:</span>{" "}
                  <span className="font-medium text-charcoal">
                    {selectedDate && `${selectedDate.getDate()} ${MONTHS_ES[selectedDate.getMonth()]}`} · {selectedTime}
                  </span>
                </div>
                <div>
                  <span className="text-charcoal/50">Total:</span>{" "}
                  <span className="font-serif text-lg text-gold-dark">{formatPrice(totalPrice)}</span>
                </div>
              </div>

              <div className="grid grid-cols-1 sm:grid-cols-2 gap-5 mb-5">
                <div>
                  <label className="block text-sm font-medium text-charcoal mb-1.5">Nombre <span className="text-rose-accent">*</span></label>
                  <input type="text" required value={form.name}
                    onChange={(e) => setForm({ ...form, name: e.target.value })}
                    className="w-full px-4 py-3 rounded-lg border border-charcoal/15 bg-white focus:border-gold focus:ring-1 focus:ring-gold/30 outline-none transition-all text-charcoal"
                    placeholder="Tu nombre" />
                </div>
                <div>
                  <label className="block text-sm font-medium text-charcoal mb-1.5">Apellidos <span className="text-rose-accent">*</span></label>
                  <input type="text" required value={form.lastName}
                    onChange={(e) => setForm({ ...form, lastName: e.target.value })}
                    className="w-full px-4 py-3 rounded-lg border border-charcoal/15 bg-white focus:border-gold focus:ring-1 focus:ring-gold/30 outline-none transition-all text-charcoal"
                    placeholder="Tus apellidos" />
                </div>
                <div>
                  <label className="block text-sm font-medium text-charcoal mb-1.5">Teléfono <span className="text-rose-accent">*</span></label>
                  <input type="tel" required value={form.phone}
                    onChange={(e) => setForm({ ...form, phone: e.target.value })}
                    className="w-full px-4 py-3 rounded-lg border border-charcoal/15 bg-white focus:border-gold focus:ring-1 focus:ring-gold/30 outline-none transition-all text-charcoal"
                    placeholder="600 000 000" />
                </div>
                <div>
                  <label className="block text-sm font-medium text-charcoal mb-1.5">Email <span className="text-charcoal/30">(opcional)</span></label>
                  <input type="email" value={form.email}
                    onChange={(e) => setForm({ ...form, email: e.target.value })}
                    className="w-full px-4 py-3 rounded-lg border border-charcoal/15 bg-white focus:border-gold focus:ring-1 focus:ring-gold/30 outline-none transition-all text-charcoal"
                    placeholder="correo@ejemplo.com" />
                </div>
              </div>
              <div className="mb-5">
                <label className="block text-sm font-medium text-charcoal mb-1.5">Notas <span className="text-charcoal/30">(opcional)</span></label>
                <textarea value={form.notes}
                  onChange={(e) => setForm({ ...form, notes: e.target.value })}
                  className="w-full px-4 py-3 rounded-lg border border-charcoal/15 bg-white focus:border-gold focus:ring-1 focus:ring-gold/30 outline-none transition-all text-charcoal min-h-[80px] resize-y"
                  placeholder="Si necesitas comentarnos algo..." />
              </div>
              <label className="flex items-center gap-3 cursor-pointer select-none">
                <input type="checkbox" checked={wantsReminder}
                  onChange={(e) => setWantsReminder(e.target.checked)}
                  className="w-5 h-5 rounded border-charcoal/20 text-gold focus:ring-gold accent-gold" />
                <span className="text-sm text-charcoal/70">
                  Quiero recibir un recordatorio por WhatsApp un día antes.
                </span>
              </label>
            </form>
          )}
        </div>

        {/* Barra inferior */}
        <div className="px-6 sm:px-10 py-5 border-t border-charcoal/8 bg-ivory/40 flex items-center justify-between">
          {step > 1 ? (
            <button onClick={goBack}
              className="inline-flex items-center gap-1 text-sm font-medium text-charcoal/60 hover:text-charcoal transition-colors">
              <ChevronLeft className="size-4" />Volver
            </button>
          ) : <div />}

          <div className="flex items-center gap-5">
            {selectedIds.length > 0 && (
              <span className="hidden sm:inline text-sm text-charcoal/50">
                {selectedServices.length} {selectedServices.length === 1 ? "servicio" : "servicios"} · <span className="font-serif text-base text-gold-dark">{formatPrice(totalPrice)}</span>
              </span>
            )}
            {step < 4 ? (
              <button onClick={goNext}
                className="inline-flex items-center gap-2 px-7 py-3 bg-charcoal text-white text-sm font-medium rounded-full hover:bg-gold transition-colors shadow-sm">
                Siguiente <ArrowRight className="size-4" />
              </button>
            ) : (
              <button type="submit" form="booking-form"
                className="inline-flex items-center gap-2 px-8 py-3 bg-gold text-white text-sm font-medium rounded-full hover:bg-gold-dark transition-all shadow-lg shadow-gold/20">
                Enviar solicitud
              </button>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
