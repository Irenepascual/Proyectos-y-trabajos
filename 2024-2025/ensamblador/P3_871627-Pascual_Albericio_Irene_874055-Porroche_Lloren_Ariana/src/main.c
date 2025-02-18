/* *****************************************************************************
  * P.H.2024: Driver/Manejador de los Leds
	*
	* blink practica 2 de proyecto hardware 2024
 */
 
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "hal_gpio.h"
#include "drv_leds.h"
#include "drv_tiempo.h"
#include "drv_consumo.h"
#include "rt_fifo.h"
#include "test_rt_fifo.h"
#include "drv_monitor.h"

#define RETARDO_MS 500 		//retardo blink en milisegundos

/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * retardo por bucle de instrucciones, solo usa el manejador del led
 * para realizar la primera sesión de la practica
 */
void blink_v1(uint32_t id){
  while (1) {
    uint32_t volatile tmo;
    
    tmo = 10000000;
    while (tmo--);
    drv_led_conmutar(id);     
	}		
}


/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * activacion por tiempo, usa tanto manejador del led como el del tiempo
 * para realizar en la segunda sesión de la practica, version a entregar
 */
void blink_v2(uint32_t id){
	
	// para medir consumo
	drv_tiempo_iniciar();
	drv_consumo_iniciar(3);	// iniciamos el monitor 3
	
	Tiempo_ms_t siguiente_activacion;	
	
	drv_led_encender(id);

	siguiente_activacion = drv_tiempo_actual_ms();
	
	/* Toggle LEDs. */
	while (true) {
		siguiente_activacion += RETARDO_MS; //ms
		drv_tiempo_esperar_hasta_ms(siguiente_activacion);
		drv_led_conmutar(id);
		//otras cosas
	}
}


/* *****************************************************************************
 */

void leds_c(uint32_t id, Tiempo_ms_t ms) {
	drv_led_conmutar(id);
}

/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * activacion por tiempo, usa tanto manejador del led como el del tiempo
 * para realizar en la tercera sesión de la practica, version a entregar
 */
void blink_v3(uint32_t id){
	drv_tiempo_iniciar();
	drv_monitor_iniciar_unico(3);	// iniciamos el monitor 3
	
	drv_led_encender(id);
	drv_tiempo_periodico_ms(RETARDO_MS, leds_c, id);
	/* Toggle LEDs. */
	while (1) {
		drv_consumo_esperar();
	}
}

/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * activacion por tiempo, usa tanto manejador del led como el del tiempo
 * para realizar en la tercera sesión de la practica, version a entregar
 */
void blink_v4(uint32_t id){
	drv_tiempo_iniciar();
	rt_FIFO_inicializar(MONITOR4);
	
	drv_led_encender(id);
	drv_tiempo_periodico_ms(RETARDO_MS, rt_FIFO_encolar, ev_T_PERIODICO);
	
	while (1) {
		EVENTO_T EV_ID_evento;
		uint32_t EV_auxData;
		Tiempo_us_t EV_TS;
		if (rt_FIFO_extraer(&EV_ID_evento, & EV_auxData, & EV_TS)){
			if (EV_ID_evento == ev_T_PERIODICO){
				drv_led_conmutar(id);
			}
		}
		else{
			drv_consumo_esperar();
		}
	}
}

/* *****************************************************************************
 * MAIN, Programa principal.
 * para la primera sesion se debe usar la funcion de blink_v1 sin temporizadores
 * para la entrega final se debe incocar a blink_v2
 */
int main(void){
	
	//////////PRUEBA DE OVERFLOW AL ENCOLAR EVENTOS://////////
	//run_tests(MONITOR4);
  ////////////////////////////////////////////// 	
	
	uint32_t Num_Leds;

	
	hal_gpio_iniciar();	// llamamos a iniciar gpio antesde que lo hagan los drivers
	
	/* Configure LED */
	Num_Leds = drv_leds_iniciar();
	
	if (Num_Leds > 0){

		//blink_v1(1);			// sesion 1 de practica 2
		//blink_v2(2);			  // para la sesion 2 de practica 2
		//blink_v3(3);
		blink_v4(4);
		//blink_v3_bis(3);
	}
}
