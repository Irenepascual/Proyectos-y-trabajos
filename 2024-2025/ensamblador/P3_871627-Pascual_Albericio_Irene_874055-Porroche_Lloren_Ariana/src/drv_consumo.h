/* *
* P.H.2024:
*/

#ifndef DRV_CONSUMO
#define DRV_CONSUMO

#include <stdint.h>

void drv_consumo_iniciar(uint32_t id_monitor);

void drv_consumo_esperar(void);

void drv_consumo_dormir(void);

#endif
