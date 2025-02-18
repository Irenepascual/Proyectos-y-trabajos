#include "rt_fifo.h"

 // Tamaño máximo de la cola

static EVENTO fifo[FIFO_TAM];           							// Cola de eventos
static indice_cola_t ultimo_tratado = 0;  						// Índice del último evento tratado
static indice_cola_t siguiente_a_tratar = 0; 					// Índice del siguiente evento a tratar
static HAL_GPIO_PIN_T overflow_monitor_pin;  					// Pin del monitor para overflow
static uint32_t  contador_eventos[EVENT_TYPES] = {0}; // Arreglo para contar cada tipo de evento


void rt_FIFO_inicializar(HAL_GPIO_PIN_T pin_monitor_overflow){
	ultimo_tratado = 0;
  siguiente_a_tratar = 0;
  overflow_monitor_pin = pin_monitor_overflow;
	
	hal_gpio_iniciar_monitor_unico(overflow_monitor_pin);

  contador_eventos[ev_VOID] = 0;
	contador_eventos[ev_T_PERIODICO] = 0;
	contador_eventos[ev_PULSAR_BOTON] = 0;
}

void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData){
	indice_cola_t siguiente_pos = (siguiente_a_tratar + 1);
	if (siguiente_pos == FIFO_TAM){
		siguiente_pos=0;
	}

  // Verifica si la cola está llena (overflow)
  if (siguiente_pos == ultimo_tratado) {
     // Overflow: activa el monitor y entra en un bucle infinito
     hal_gpio_monitor_marcar(overflow_monitor_pin);
     while (1) {
            // Bucle infinito debido al overflow
     }
	 }

   // Encola el evento
  fifo[siguiente_a_tratar].ID_EVENTO = (EVENTO_T)ID_evento;
  fifo[siguiente_a_tratar].auxData = auxData;
  fifo[siguiente_a_tratar].TS = drv_tiempo_actual_us();  // Marca de tiempo actual
	
	if (ID_evento<EVENT_TYPES){
		contador_eventos[ID_evento]++;
	}
	
  // Actualiza el índice `siguiente_a_tratar`
	
  siguiente_a_tratar = siguiente_pos;
}

uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData, Tiempo_us_t *TS){
	if (ultimo_tratado == siguiente_a_tratar) {
     // La cola está vacía, no hay eventos para extraer
     return 0;
  }
	
  // Extrae el evento del índice `ultimo_tratado`
  *ID_evento = fifo[ultimo_tratado].ID_EVENTO;
  *auxData = fifo[ultimo_tratado].auxData;
  *TS = fifo[ultimo_tratado].TS;

  // Actualiza `ultimo_tratado` para que avance al siguiente evento en la cola
  ultimo_tratado = (ultimo_tratado + 1);
	if (ultimo_tratado == FIFO_TAM){
		ultimo_tratado = 0;
	}
	
	return 1;

//	if (ultimo_tratado < siguiente_a_tratar){
//		return (siguiente_a_tratar-ultimo_tratado);
//	}
//	
//  return (ultimo_tratado-siguiente_a_tratar);
}

uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento){
	    if (ID_evento < EVENT_TYPES) {
        return contador_eventos[ID_evento];
    }
    return 0; 
}
