/* 
 * P.H.2024: Módulo para el consumo en el LPC2105
 */

#include "hal_consumo.h"

#include <LPC210x.H>                       

//definida en Startup.s
extern void switch_to_PLL(void);
extern void Reset_Handler(void);

/* 
 * No hace nada
 */
void hal_consumo_iniciar(void) {
}

/*
 * Pone al procesador en estado de espera para reducir su consumo
 */
void hal_consumo_esperar(void)  {
  EXTWAKE = 7; 			// (0x111) --> configura los pines EXTINT0, EXTINT1 y EXTINT2
										// para despertar al procesador gracias a su interrupción
	
	PCON |= 0x01; 		// Establece el bit 0 (0x1) del registro `PCON` para poner el 
										// procesador en modo "espera" (Idle). Saldrá de espera con interrupción
}

/* 
 * Duerme al procesador para minimizar su consumo
 */
void hal_consumo_dormir(void)  {	
	EXTWAKE = 7; 			// (0x7) --> los botones conectados a EXTINT0, EXTINT1 y EXTINT2
										// podrán despertar al procesador (BUTTON_1, BUTTON_2 y BUTTON_3)
	
	PCON |= 0x02; 		// Establece el bit 1 (0x10) del registro `PCON` para poner el 
										// procesador en modo "dormir" (PowerDown). 
	
	switch_to_PLL(); //PLL aranca a 12Mhz cuando volvemos de powewr down
}

