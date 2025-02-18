/* *
* P.H.2024:
*/

#include "drv_consumo.h"
#include "drv_monitor.h"
#include "hal_consumo.h"
#include "hal_gpio.h"
#include "board.h"

#if MONITOR_NUMBER > 0
	static const uint8_t monitor_list[MONITOR_NUMBER] = MONITOR_LIST;
#endif


void drv_consumo_iniciar(uint32_t id_monitor){}

void drv_consumo_esperar(void){
	drv_monitor_marcar(monitor_list[3]);	// para medir consumo
	
	hal_consumo_esperar();

	drv_monitor_desmarcar(monitor_list[3]);	// para medir consumo
}

void drv_consumo_dormir(void){
	drv_monitor_marcar(monitor_list[3]);	// para medir consumo
	
	hal_consumo_dormir();
	
	drv_monitor_desmarcar(monitor_list[3]);	// para medir consumo
}
