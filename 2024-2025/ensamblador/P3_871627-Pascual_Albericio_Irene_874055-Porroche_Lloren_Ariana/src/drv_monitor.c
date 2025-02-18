#include "drv_monitor.h"
#include "board.h"
#include "hal_gpio.h"


/**
 * inicializa los monitores 
 */
 
#if MONITOR_NUMBER > 0
	static const uint8_t monitor_list[MONITOR_NUMBER] = MONITOR_LIST;
#endif

uint32_t drv_monitor_iniciar(){
	#if MONITOR_NUMBER > 0
			hal_gpio_iniciar_monitor();
  #endif //MONITOR_NUMBER > 0	
	
	return MONITOR_NUMBER;  //definido en board_xxx.h en cada placa... 
}

uint32_t drv_monitor_iniciar_unico(uint32_t id){
	#if MONITOR_NUMBER > 0
			if ((id <= MONITOR_NUMBER) && (id >0)) hal_gpio_iniciar_monitor_unico(monitor_list[id-1]);
  #endif //MONITOR_NUMBER > 0	
		
	return MONITOR_NUMBER;
}


void drv_monitor_marcar(uint32_t id){
	hal_gpio_monitor_marcar(id);
}
void drv_monitor_desmarcar(uint32_t id){
	hal_gpio_monitor_desmarcar(id);
}
