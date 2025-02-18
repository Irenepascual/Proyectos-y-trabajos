/* *****************************************************************************
 * P.H.2024: Watchdog para el LPC2105
 */

#include "hal_WDG.h"
#include "rt_sc.h"
#include <LPC210x.H>                       /* LPC210x definitions */

/* 
 * Esta función configura el Watchdog Timer para que reinicie el sistema si no 
 * se "alimentado" antes de que se agote el tiempo especificado
 */
void hal_WDG_iniciar(uint32_t sec) {
	WDTC = sec * (15000000 / 4);	// Tiempo hasta que expire el temporizador del watchdog
	WDMOD = 0x03;		// se configura que cuando expire el tiempo se reinicie el sistema
									// -> bit 0 : habilita watchdog timer
									// -> bit 1 : configura reinicio al expirar
	hal_WDG_feed();	// alimenta para que empiece a contar ya
}

/*
 * Función que se llama periodicamente para "alimentar" y se verifique
 * que el sistema sigue activo
 */
void hal_WDG_feed(void) {
	entrar_SC();
	WDFEED = 0xAA; // Primer valor para alimentar el Watchdog.
	WDFEED = 0x55; // Segundo valor para alimentar el Watchdog.
	salir_SC();
}


