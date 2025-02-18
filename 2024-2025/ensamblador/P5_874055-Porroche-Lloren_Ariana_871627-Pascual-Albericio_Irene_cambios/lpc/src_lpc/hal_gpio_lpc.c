/* *****************************************************************************
 * P.H.2024: GPIOs en LPC2105
 * implementacion para cumplir el hal_gpio.h
 * interrupciones externas para los botones lo dejamos para otro modulo aparte
 */
 
#include "hal_gpio.h"

#include <LPC210x.H>                       /* LPC210x definitions */

#define IODIR0 			(*((volatile unsigned char *) 0xE0028008))

/**
 * Permite emplear el GPIO y debe ser invocada antes
 * de poder llamar al resto de funciones de la biblioteca.
 * re-configura todos los pines como de entrada (para evitar cortocircuitos)
 */
void hal_gpio_iniciar(void){
  // Reiniciamos los pines todos como salida (igual al reset):
  IODIR = 0x0; // GPIO Port Direction control register.
				       // Controla la dirección de cada puerto pin
}



/**
 * El gpio se configuran como entrada o salida según la dirección.
 */
void hal_gpio_sentido(HAL_GPIO_PIN_T gpio, hal_gpio_pin_dir_t direccion){
	uint32_t masc = (1UL << gpio);
	if (direccion == HAL_GPIO_PIN_DIR_INPUT){
			IODIR = IODIR & ~masc;
	}
	else if (direccion == HAL_GPIO_PIN_DIR_OUTPUT){
			IODIR = IODIR | masc;
	}		
}
 
/**
 * La función devuelve del led indicado un entero de si está encendido (1) o apagado (0)
 */
uint32_t hal_gpio_leer(HAL_GPIO_PIN_T gpio){
	uint32_t masc = (1UL << gpio);	// máscara de la posición 1 del gpio
	return ((IOPIN & masc)!=0);			// aplicamos la máscara para ver si está encendido o apagado
}


/**
 * Escribe en el gpio el valor
 */
void hal_gpio_escribir(HAL_GPIO_PIN_T gpio, uint32_t valor){
	uint32_t masc = (1UL << gpio); 					// máscara de la posición 1 del gpio
	
	if ((valor & 0x01) == 0) IOCLR = masc;	// verifica el valor a escribir en el pin, si el valor es 0, escribirá un 0
	else IOSET = masc;											// sino escribirá un 1
}


/*
 * Los bits indicados se configuran como
 * entrada o salida según la dirección.
 */
void hal_gpio_sentido_n(HAL_GPIO_PIN_T gpio_inicial, uint8_t num_bits, hal_gpio_pin_dir_t direccion){
	// máscara para seleccionar los pines deseados (desde el bit gpio_inicial hasta num_bits posiciones)						
	uint32_t masc = ((1 << num_bits) - 1) << gpio_inicial;
				
	// Si es de entrada
	if (direccion == HAL_GPIO_PIN_DIR_INPUT){
			IODIR = IODIR & ~masc;	// Pone los bits seleccionados de modo entrada
	}
	// Si es de salida
	else if (direccion == HAL_GPIO_PIN_DIR_OUTPUT){
			IODIR = IODIR | masc;		// Pone los bits seleccionados de modo salida
	}									
}		

/**
 * La función devuelve un entero con el valor de los bits indicados.
 * Ejemplo:
 *		- valor de los pines: 0x0F0FAFF0
 *		- bit_inicial: 12 num_bits: 4
 *		- valor que retorna la función: 10 (lee los 4 bits 12-15)
 */
uint32_t hal_gpio_leer_n(HAL_GPIO_PIN_T gpio_inicial, uint8_t num_bits){		
	// máscara para seleccionar los pines deseados (desde el bit gpio_inicial hasta num_bits posiciones)			
	uint32_t masc = ((1 << num_bits) - 1) << gpio_inicial;

	return (IOPIN & masc) >> gpio_inicial;
  	// IOPIN : GPIO Port Pin value register. Contiene el estado de los
	  // puertos pines configurados independientemente de la direccion.
}

/**
 * Escribe en los bits indicados el valor 
 * (si valor no puede representarse en los bits indicados,
 *  se escribirá los num_bits menos significativos a partir del inicial).
 */
void hal_gpio_escribir_n(HAL_GPIO_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor){
	// obtiene los pins necesarios del valor y los desplaza a la posición inicial 
	uint32_t masc_value = (valor & ((1 << num_bits) - 1)) << bit_inicial;
	
	// máscara para seleccionar los pines deseados (desde el bit gpio_inicial hasta num_bits posiciones)				
	uint32_t masc = ((1 << num_bits) - 1) << bit_inicial;
	
	// Limpia los bits correspondientes en el registro "IOPIN" antes de escribir	
	uint32_t temp = IOPIN & ~masc;
	
	// Actualiza "IOPIN" escribiendo los nuevos valores en los bits seleccionados	
	IOPIN = temp | masc_value; 
	// limpia la mascara en el iopin y cambia sus bits de golpe
}


/*
 * No hace nada
 */
void hal_gpio_iniciar_monitor(void){
}

/*
 * Inicializa a estado bajo el monitor con identificador id
 */
void hal_gpio_iniciar_monitor_unico(uint32_t id){
  IOCLR = (1 << id);  // Asegura que el pin está en bajo inicialmente
}

/*
 * Pone a estado bajo el monitor con identificador id
 */
void hal_gpio_monitor_marcar(uint32_t id){
	IOSET = (1UL << id); // Activa el monitor (marca el pin en alto)
}

/*
 * Pone a estado alto el monitor con identificador id
 */
void hal_gpio_monitor_desmarcar(uint32_t id){
	IOCLR = (1UL << id); // Desactiva el monitor (marca el pin en bajo)
}

/*
 * Inicializa los botones del lpc (son 3) como entradas
 */
void hal_gpio_botones_iniciar(void) {
	// ACTIVAR PIN P0.16 (BUTTON_3)    	
	PINSEL1 |= 0x00000001; // P0.16 (sets bits 1:0)

	// ACTIVAR PIN P0.15 (BUTTON_2)
	PINSEL0 |= 0x80000000; // P0.15 (sets bits 31:30)

	// ACTIVAR PIN P0.14 (BUTTON_1)
	PINSEL0 |= 0x20000000; // P0.14 (sets bits 29:28)
}







