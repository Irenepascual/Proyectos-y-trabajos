#include "test_rt_fifo.h"

void run_tests(HAL_GPIO_PIN_T pin_monitor_overflow) {
    HAL_GPIO_PIN_T pin = pin_monitor_overflow;
    rt_FIFO_inicializar(pin);

    uint32_t auxData = 100; // valor auxiliar para cada evento

    // Test básico de encolar eventos hasta antes de overflow (FIFO_TAM-1 eventos)
    for (int i = 0; i < FIFO_TAM-1; i++) {
        rt_FIFO_encolar(ev_T_PERIODICO, auxData + i);
    }
		
		// Revisa las estadísticas
    uint32_t num_VOID = rt_FIFO_estadisticas(ev_VOID);
    uint32_t num_T_PERIODICO = rt_FIFO_estadisticas(ev_T_PERIODICO);
    uint32_t num_PULSAR_BOTON = rt_FIFO_estadisticas(ev_PULSAR_BOTON);
		
		// Test básico de overflow (FIFO_TAM eventos en la cola)
		rt_FIFO_encolar(ev_T_PERIODICO, auxData + FIFO_TAM-1);
}

