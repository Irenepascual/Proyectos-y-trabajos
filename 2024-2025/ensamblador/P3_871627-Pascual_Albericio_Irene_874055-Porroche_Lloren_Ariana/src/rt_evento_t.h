#ifndef RT_EVENTO_T
#define RT_EVENTO_T


typedef enum {
	ev_VOID = 0, // default, vacio
	ev_T_PERIODICO = 1, //
	ev_PULSAR_BOTON = 2,
} EVENTO_T; //mapea a uint32_t

#define EVENT_TYPES 3


#endif
