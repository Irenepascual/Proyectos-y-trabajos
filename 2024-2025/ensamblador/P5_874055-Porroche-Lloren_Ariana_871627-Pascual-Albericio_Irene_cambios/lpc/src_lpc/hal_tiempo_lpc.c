/* *****************************************************************************
 * P.H.2024: Temporizadores en LPC2105, Timer 0 y Timer 1
 * implementacion para cumplir el hal_tiempo.h
 * Timer0 cumple maxima frecuencia, minimas interrupciones contando en ticks
 * Timer1 avisa cada periodo de activacion en ticks
 */
 
#include "hal_tiempo.h" 

#include <LPC210x.H> /* LPC210x definitions */

#include <stdint.h>
#include <stdbool.h>

#define MAX_COUNTER_VALUE 0xFFFFFFFE			// Maximo valor del contador de 32 bits
#define HAL_TICKS2US			15 							// funcionamos PCLK a 15 MHz de un total de 60 MHz CPU Clock
#define US2MS							1000						//milisegundos por microsogundos



static volatile uint32_t timer0_int_count = 0;	// contador de 32 bits de veces que ha saltado la RSI Timer0

/* 
 * Timer 0 Interrupt Service Routine
 * Incrementa el contador de interrupciones y limpia las interrupciones.
 */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;               // Limpia la bandera de interrupción
    VICVectAddr = 0;        // indica que la interrupción actual ya ha sido atendida
}

/* 
 * Programa un contador de tick sobre Timer0, con maxima precisión y minimas interrupciones
 */
uint32_t hal_tiempo_iniciar_tick() {
	timer0_int_count = 0;					// Resetea el contador de interrupciones a 0.	
	T0MR0 = MAX_COUNTER_VALUE;		// Interrupción cuando TC alcanza el valor máximo
  T0MCR = 3;              			// Configuración binaria: (0x11) (interrupción y reinicio).
	
	// Configura la dirección del vector de interrupción para el Timer 0.	
	VICVectAddr0 = (unsigned long)timer0_ISR;    // interrupción tratada por timer0_ISR
  
	// 0x20 --> bit 5 : habilita interrupcciones vectorizadas 
	// 4 es el número de la interrupción que corresponde al timer0
	VICVectCntl0 = 0x20 | 4;                   
  VICIntEnable = VICIntEnable | 0x00000010;   // Habilita interrupción 4 (operaciones de arriba) del timer0 

	T0TCR = 2; // Reincia los contadores
  T0TCR = 1;  // Empieza la cuenta del timer0
	
	return HAL_TICKS2US;  		//devuelve el factor conversion de ticks a microsegundos de este hardware
}


/**
 * Lee el tiempo que lleva contando el contador y lo devuelve en ticks.
 */
uint64_t hal_tiempo_actual_tick() { 
	uint64_t time;
  time = ((MAX_COUNTER_VALUE+1)*timer0_int_count) + (uint64_t)T0TC; 
	return time;
}


/* *****************************************************************************
 */

static void(*f_callbacks[2])();  // Vector de funciones de callback


void timer0__ISR (void) __irq {
	if (T0IR & 0x01) {
    T0IR = 0x01;  // Limpia la bandera de interrupción
	}
	f_callbacks[0]();
	T0IR = 1;						// Limpia la bandera de interrupción
	VICVectAddr = 0;		// indica que la interrupción actual ya ha sido atendida
}

void timer1__ISR (void) __irq {
	if (T1IR & 0x01) {
    T1IR = 0x01;  // Limpia la bandera de interrupción
	}
	f_callbacks[1]();
	T1IR = 1;						// Limpia la bandera de interrupción
	VICVectAddr = 0;		// indica que la interrupción actual ya ha sido atendida
}


/* 
 * Programa el reloj para que llame a la función de callback cada periodo.
 * El periodo se indica en tick. Si el periodo es cero se para el temporizador. 
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)(), uint32_t id){
  
	f_callbacks[1] = funcion_callback;
	
	if (periodo_en_tick != 0) { //Si el periodo es cero solo se para el temporizador.
		T1MR0 = periodo_en_tick  - 1; 		// 15 Ticks (ciclos) por microsegundo.
																			// (periodo_en_ms * HAL_TICKS2US * US2MS) - 1;  
																			// resto uno por como incrementa y compara
		// ALARMA PERIÓDICA
		T1MCR = 3;  

		// Habilita la interrupción del Timer1
		VICVectAddr1 = (unsigned long)timer1__ISR;
		// 0x20 --> bit 5 : habilita interrupcciones vectorizadas 
		// 5 es el número de la interrupción que corresponde al timer1
		VICVectCntl1 = 0x20 | 5;                   
		VICIntEnable = VICIntEnable | 0x00000020;  
		
		T1TCR = 3;  // Reincia los contadores
		T1TCR = 1;  // Empieza la cuenta
	} else {
		// Detiene el temporizador
		T1TCR = 0;  
    VICIntEnClr = 0x20;   // Deshabilita la interrupción del Timer 1
	}
}





// **********************************************************************
// *********************** NUEVO PARA EL BIS ****************************
// **********************************************************************

static void(*f_callbacks_lpc_nuevas[2])();

void timer0___ISR (void) __irq {
	if (T0IR & 0x01) {
    T0IR = 0x01;  // Limpia la bandera de interrupción
	}
	f_callbacks_lpc_nuevas[0](0,0);
	T0IR = 1;						// Limpia la bandera de interrupción
	VICVectAddr = 0;		// indica que la interrupción actual ya ha sido atendida
}

void timer1___ISR (void) __irq {
	if (T1IR & 0x01) {
    T1IR = 0x01;  // Limpia la bandera de interrupción
	}
	f_callbacks_lpc_nuevas[1](1,0);
	T1IR = 1;						// Limpia la bandera de interrupción
	VICVectAddr = 0;		// indica que la interrupción actual ya ha sido atendida
}


/*
 * Programa el temporizador indicado en 'timer'
 */
void hal_tiempo_reloj_tick(uint8_t timer, uint32_t periodo_en_tick, bool esPeriodica, void(*funcion_callback)()) {

	// Activamos la alarma
	if (periodo_en_tick != 0) {
		if (timer == 0) {
			f_callbacks_lpc_nuevas[0] = funcion_callback;	// guardamos la función de callback a la que debe llamar
																					// cuando llegue la interrupción
			// Limpiar interrupciones del temporizador (si están activas)
			T0IR = 0xFF;  // Limpiar todas las interrupciones pendientes del Timer 0
			T0MR0 = periodo_en_tick  - 1;
			// Configura el modo de interrupción:
      // - Si es periódica: genera interrupción y reinicia al alcanzar MR0.
      // - Si no es periódica: solo genera una interrupción sin reiniciar.
			if (esPeriodica) {
				T0MCR = 3;
			} else {
				T0MCR = 1;
			}
			VICVectAddr0 = (unsigned long)timer0___ISR;
			VICVectCntl0 = 0x20 | 4;                   
			VICIntEnable |= 0x00000010;  
			T0TCR = 3;
			T0TCR = 1;
		}
		else if (timer == 1) {
			f_callbacks_lpc_nuevas[1] = funcion_callback;	// guardamos la función de callback a la que debe llamar
																					// cuando llegue la interrupción
			// Limpiar interrupciones del temporizador (si están activas)
			//T1TCR = 0;  // Detener el temporizador
			T1IR = 0xFF;  // Limpiar todas las interrupciones pendientes
			//T1MCR = 0;  // Limpiar cualquier configuración previa del registro MCR			
			T1MR0 = periodo_en_tick  - 1;
			
			// Configura el modo de interrupción:
      // - Si es periódica: genera interrupción y reinicia al alcanzar MR1.
      // - Si no es periódica: solo genera una interrupción sin reiniciar.
			if (esPeriodica) {
				T1MCR = 3;
			} else {
				T1MCR = 1;
			}
			VICVectAddr1 = (unsigned long)timer1___ISR;
			VICVectCntl1 = 0x20 | 5;                   
			VICIntEnable |= 0x00000020;  
			T1TCR = 3;
			T1TCR = 1;	
		}
		// si quieren programar un timer fuera del rango establecido
		else {
			while(1) {
				// error: timer fuera del rango establecido
			}
		}
	}
	
	// Desactivamos la alarma
	else {
		if (timer == 0) {
			T0TCR = 0;   	// Detiene el Timer0 
			VICIntEnClr = 0x10;				// Deshabilita la interrupción del Timer0
		}
		else if (timer == 1) {
			T1TCR = 0;    	// Detiene el Timer1 
			VICIntEnClr = 0x20;				// Deshabilita la interrupción del Timer1
		}
		// si quieren programar un timer fuera del rango establecido
		else {
			while(1) {
				// error: timer fuera del rango establecido
			}
		}
	}
}
