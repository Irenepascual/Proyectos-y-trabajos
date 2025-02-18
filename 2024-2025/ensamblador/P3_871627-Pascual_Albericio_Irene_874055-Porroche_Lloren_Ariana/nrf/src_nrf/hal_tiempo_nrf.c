/* *****************************************************************************
 * P.H.2024: TODO
 * implementacion para cumplir el hal_tiempo.h
 */
 
#include <nrf.h>

#define MAX_COUNTER_VALUE 0xFFFFFFFE	// Maximo valor del contador de 32 bits
#define HAL_TICKS2US			15 	// funcionamos PCLK a 15 MHz de un total de 60 MHz CPU Clock

/* *****************************************************************************
 * Timer0 contador de ticks
 */
static volatile uint32_t timer0_int_count = 0;// contador de 32 bits de veces que ha saltado la RSI Timer0


void TIMER0_IRQHandler(void){
	if (NRF_TIMER0->EVENTS_COMPARE[0] == 1) {
		NRF_TIMER0->EVENTS_COMPARE[0] = 0;
		timer0_int_count++;
	}
}

/* *****************************************************************************
 * configura e inicializa la cuenta de tiempo en ticks del hardware y
 * devuelve la constante hal_ticks2us,
 * hal_ticks2us permite pasar de los ticks del hardware a microsegundos
 * (tip, el driver lo necesitara para trabajar en us y ms de la app y hacer la conversion a ticks del hardware)
 */
uint32_t hal_tiempo_iniciar_tick(void) {
	timer0_int_count = 0;

	NRF_TIMER0->TASKS_STOP = 1;                       // Detiene el temporizador
	NRF_TIMER0->TASKS_CLEAR = 1;                      // Limpia el contador
	NRF_TIMER0->CC[0] = MAX_COUNTER_VALUE;            // Establece el valor máximo de comparación
	NRF_TIMER0->PRESCALER = 0;                        // Configura el prescaler a 1 MHz
		
  NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
	NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;  				// Modo de temporizador
  NRF_TIMER0->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos; // Limpia automáticamente

	NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;
	NVIC_EnableIRQ(TIMER0_IRQn);
	NRF_TIMER0->TASKS_START = 1;

	return HAL_TICKS2US;
}

/* *****************************************************************************
 * nos devuelve el numero total de ticks desde que se inicio la cuenta
 */
uint64_t hal_tiempo_actual_tick(void) {
	NRF_TIMER0->TASKS_CAPTURE[0] = 1;
	uint64_t ticks_capturados = NRF_TIMER0->CC[0];

	return ((MAX_COUNTER_VALUE+1)*timer0_int_count + ticks_capturados);
}

/* *****************************************************************************
 * Timer 1 Interrupt Service Routine (ISR)
 */

static void(*f_callback)();  // Puntero a la función de callback para la interrupción


void TIMER1_IRQHandler(void) {
    if (NRF_TIMER1->EVENTS_COMPARE[1]) {
        NRF_TIMER1->EVENTS_COMPARE[1] = 0;             // Limpia la bandera de interrupción
        f_callback();                                  // Llama a la función de callback
    }
}

/* *****************************************************************************
 * TODO para la practica 3, no se si hacer hal propio...
 * programa una activacion periodica cada periodo_en_tick ticks de la maquina
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)()) {
		f_callback = funcion_callback;

    if (periodo_en_tick != 0) { // Si el periodo no es cero, se configura el temporizador
        //NRF_TIMER1->TASKS_STOP = 1;                    // Detiene el temporizador
        NRF_TIMER1->TASKS_CLEAR = 1;                   // Limpia el contador
			  NRF_TIMER1->CC[1] = periodo_en_tick - 1;       // Configura el valor de comparación
			  NRF_TIMER1->PRESCALER = 0;                     // Prescaler para 1 MHz (1 us por tick)
			
        NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
				NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;  				// Modo de temporizador

        NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE1_CLEAR_Enabled << TIMER_SHORTS_COMPARE1_CLEAR_Pos; // Limpia automáticamente

        // Habilita la interrupción para el evento de comparación
        NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos;
        NVIC_EnableIRQ(TIMER1_IRQn);                   // Habilita la interrupción en el NVIC

        NRF_TIMER1->TASKS_START = 1;                   // Inicia el temporizador
    } else {
        // Detener el temporizador y deshabilitar interrupciones si el período es cero
        NRF_TIMER1->TASKS_STOP = 1;
        NVIC_DisableIRQ(TIMER1_IRQn);                  // Deshabilita la interrupción
    }	
}





