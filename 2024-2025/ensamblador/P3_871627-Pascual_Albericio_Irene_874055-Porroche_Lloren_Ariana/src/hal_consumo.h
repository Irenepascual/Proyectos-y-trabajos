/* *
* P.H.2024: hal_tiempos, interface que nos independiza del hardware 
*/

#ifndef HAL_CONSUMO
#define HAL_CONSUMO

#include <stdint.h>


void hal_consumo_iniciar(void);

void hal_consumo_esperar(void);

void hal_consumo_dormir(void);

#endif
