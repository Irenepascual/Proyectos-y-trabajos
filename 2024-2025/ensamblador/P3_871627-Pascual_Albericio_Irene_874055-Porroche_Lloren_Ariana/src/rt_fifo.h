#ifndef RT_FIFO
#define RT_FIFO

#include <stdint.h>
#include "hal_gpio.h"
#include "board.h"
#include "rt_evento_t.h"
#include "drv_tiempo.h"

#define FIFO_TAM 32

typedef struct {
	EVENTO_T ID_EVENTO;
	uint32_t auxData;
	Tiempo_us_t TS;
} EVENTO;

typedef uint8_t indice_cola_t;

void rt_FIFO_inicializar(HAL_GPIO_PIN_T pin_monitor_overflow);
void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData);
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData, Tiempo_us_t *TS);
uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento);

#endif
