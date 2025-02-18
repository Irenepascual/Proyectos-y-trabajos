/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los Leds
 * suministra los servicios de iniciar, encender, apagar, conmutar... independientemente del hardware
 */

#ifndef DRV_MONITOR
#define DRV_MONITOR

#include <stdint.h>

/**
 * inicializa los monitores 
 */
uint32_t drv_monitor_iniciar(void);

uint32_t drv_monitor_iniciar_unico(uint32_t id);

void drv_monitor_marcar(uint32_t id);
void drv_monitor_desmarcar(uint32_t id);
	
#endif
