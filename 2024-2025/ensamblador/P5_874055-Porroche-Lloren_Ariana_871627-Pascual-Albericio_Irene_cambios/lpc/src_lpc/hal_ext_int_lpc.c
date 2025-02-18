/* 
 * P.H.2024: Módulo para gestionar las interrupciones en el LPC2105
 */
 
#include "hal_ext_int.h"

#include <LPC210x.H>

#define EXTMODE   		 (*((volatile unsigned char *) 0xE01FC148))
#define EXTPOLAR  		 (*((volatile unsigned char *) 0xE01FC14C))

void (*ptr_hal_ext_int_lpc)();

/* 
 * Deshabilita las interrupciones de los botones
 */
void hal_ext_int_deshabilitar_botones_irqs(void) {
	VICIntEnClr = 0x0001C000;	 // deshabilita EINT0, EINT1 y EINT2
}

/* 
 * Habilita las interrupciones de los botones
 */
void hal_ext_int_habilitar_botones_irqs(void) {
	// Limpia los flags pendientes de las interrupciones externas
  EXTINT |= 0x00000007;  // Borra los flags de EINT0 (bit 0), EINT1 (bit 1), EINT2 (bit 2)
	
	
  VICIntEnable |= 0x00004000;	// habilita EINT0
	VICIntEnable |= 0x00008000;	// habilita EINT1
  VICIntEnable |= 0x00010000;	// habilita EINT2
}

/* 
 * Deshabilita las interrupciones del botón que le pase como argumento
 */
void hal_ext_int_deshabilitar_boton_irqs(uint8_t boton) {
	if (boton == 3) {
		VICIntEnClr = 0x00004000;  // deshabilita EINT0
	}
	else if (boton == 1) {
		VICIntEnClr = 0x00008000;  // deshabilita EINT1
	}
	else if (boton == 2) {
		VICIntEnClr = 0x00010000;  // deshabilita EINT2
	}
}

/* 
 * Habilita las interrupciones del botón que le pase como argumento
 */
void hal_ext_int_habilitar_boton_irqs(uint8_t boton) {
	if (boton == 3) {
		VICIntEnable |= 0x00004000;	// Habilita la interrupción de EXTINT0
	}
	else if (boton == 1) {
		VICIntEnable |= 0x00008000;	// Habilita la interrupción de EXTINT1
	}
	else if (boton == 2) {
		VICIntEnable |= 0x00010000;	// Habilita la interrupción de EXTINT2
	} 
}


void eint0_ISR (void) __irq {
	hal_ext_int_deshabilitar_boton_irqs(3);
	
	EXTINT |= 1;     	// borra el flag de interrupcion de EXTINT0  
	VICVectAddr = 0; 	// indica que la interrupción actual ya ha sido atendida
	
  (*ptr_hal_ext_int_lpc)(3);
}

void eint1_ISR (void) __irq {
	hal_ext_int_deshabilitar_boton_irqs(1);
	
	EXTINT |= 2;     	// borra el flag de interrupcion de EXTINT1  
	VICVectAddr = 0; 	// indica que la interrupción actual ya ha sido atendida

  (*ptr_hal_ext_int_lpc)(1);
}

void eint2_ISR (void) __irq {
	hal_ext_int_deshabilitar_boton_irqs(2);

  EXTINT |= 4;     	// borra el flag de interrupcion de EXTINT2
  VICVectAddr = 0; 	// indica que la interrupción actual ya ha sido atendida
	
	(*ptr_hal_ext_int_lpc)(2);
}

/* 
 * Configura las interrupciones externas (EINT0, EINT1 y EINT2) para los botones
 * y establece la función de callback que ejecutará cuando se detecten estas interrupciones
 */
void hal_ext_int_botones_iniciar(void(*funcion_callback)()) {
	ptr_hal_ext_int_lpc = funcion_callback;
	
	// Configura el modo de las interrupciones externas (EXTINT):
	EXTMODE 	=	1;	 		//Configura el modo en el que se detectan las interrupciones externas
											// 1-> sensible a bordes, así se activará cuando ocurra un cambio
											//     de estado del pin (LoToHi o HiToLo)
	
	EXTPOLAR	=	0;			// Configura la polaridad del borde
											// 0-> la interrupción se dispara de HiToLo
											// 1-> la interrupción se dispara de LoToHi 
	
	EXTINT |= 1;  			// Limpia cualquier interrupción externa pendiente

	// ACTIVAR PIN P0.16 (BUTTON_3)    	
	VICVectAddr4 = (unsigned long)eint0_ISR;
	VICVectCntl4 = 0x20 | 14;    // 14 es el número de interrupción asignado (se corresponde con el EINT0)    

	// ACTIVAR PIN P0.15 (BUTTON_2)
  VICVectAddr6 = (unsigned long)eint2_ISR;
	VICVectCntl6 = 0x20 | 16;    // 16 es el número de interrupción asignado (se corresponde con el EINT2)  

	// ACTIVAR PIN P0.14 (BUTTON_1)
	VICVectAddr5 = (unsigned long)eint1_ISR;
	VICVectCntl5 = 0x20 | 15;    // 15 es el número de interrupción asignado (se corresponde con el EINT1)  

	hal_ext_int_habilitar_botones_irqs();
}


uint8_t esta_pulsado_boton_1(void) {
	EXTINT |= 2;
  return ((EXTINT & 2) != 0);
}

uint8_t esta_pulsado_boton_2(void) {
	EXTINT |= 4;
  return ((EXTINT & 4) != 0);
}

uint8_t esta_pulsado_boton_3(void) {
	EXTINT |= 1;
  return ((EXTINT & 1) != 0);
}

/*
 * devuelve el número del botón que está pulsado, si no hay ninguno, devuelve 0.
 */
uint32_t comprobar_boton_pulsado(uint8_t boton) {
	if (boton == 1) {
		return esta_pulsado_boton_1();
	}
	if (boton == 2) {
		return esta_pulsado_boton_2();
	}
	if (boton == 3) {
		return esta_pulsado_boton_3();
	}
	return 0;	
}


uint8_t num_botones_pulsados_realmente(void) {
	uint8_t num_pulsados = 0;
	for (uint8_t i=1; i<=3; i++) {
		if (comprobar_boton_pulsado(i)) {
			num_pulsados++;
		}
	}
	return num_pulsados;
}

