/* *
* P.H.2024:
* */
 
#include <nrf.h>
#include <board_nrf52840dk.h>


#define MAX_COUNTER_VALUE 0xFFFFFFFE			// Maximo valor del contador de 32 bits
#define HAL_TICKS2US			15 							// funcionamos PCLK a 15 MHz de un total de 60 MHz CPU Clock


/* *****************************************************************************
 * Timer0 contador de ticks
 */
static volatile uint32_t timer0_int_count = 0;	// contador de 32 bits de veces que ha saltado la RSI Timer0

void hal_consumo_iniciar(void){
	// no hacemos nada
}

void hal_consumo_esperar(void){
	__WFI();
}

void hal_consumo_dormir(void){
	__WFI();
}
