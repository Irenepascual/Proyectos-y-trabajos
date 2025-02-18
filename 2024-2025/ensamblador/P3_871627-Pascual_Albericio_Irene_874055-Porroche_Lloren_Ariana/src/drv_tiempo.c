/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * suministra los servicios independientemente del hardware
 *
 * usa los servicos de hal_tiempo.h: 
 */
 
#include "drv_tiempo.h"
#include "hal_tiempo.h"
#define 	US2MS							1000						//milisegundos por microsegundos

static volatile uint32_t HAL_TICKS2US = 0;

/**
 * inicializa el reloj y empieza a contar
 */
void drv_tiempo_iniciar(void){
	HAL_TICKS2US = hal_tiempo_iniciar_tick();
}
/**
 * tiempo desde que se inicio el temporizador en microsegundos
 */
Tiempo_us_t drv_tiempo_actual_us(void){
	uint32_t ticks_actual = hal_tiempo_actual_tick();
	return ticks_actual / HAL_TICKS2US;
}

/**
 * tiempo desde que se inicio el temporizador en milisegundos
 */
Tiempo_ms_t drv_tiempo_actual_ms(void){
	uint32_t ticks_us = drv_tiempo_actual_us();
	return ticks_us / US2MS;
}

/**
 * retardo: esperar un cierto tiempo en milisegundos
 */
void drv_tiempo_esperar_ms(Tiempo_ms_t ms){
	uint32_t us_inicio = drv_tiempo_actual_us();
	uint32_t us_a_esperar = ms * US2MS;
	
	while ((drv_tiempo_actual_us() - us_inicio) < us_a_esperar);

}

/**
 * esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms){
  uint32_t us_objetivo = ms * US2MS;
    
  while (drv_tiempo_actual_us() < us_objetivo);
    
  return drv_tiempo_actual_ms();
}


void (*ptr)();
static uint32_t callback_id;  // Puntero a la función de callback para la interrupción

void funcion_que_llama() {
	(*ptr)(callback_id, drv_tiempo_actual_ms());
}

// NUEVA V3
void drv_tiempo_periodico_ms(Tiempo_ms_t ms,  void(*funcion_callback)(), uint32_t id) {
	ptr = funcion_callback;
	callback_id = id;

	hal_tiempo_reloj_periodico_tick( ms*HAL_TICKS2US*US2MS, funcion_que_llama);
}
