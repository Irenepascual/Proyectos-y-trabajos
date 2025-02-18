//*********************************************************************************
// Plantilla para implementación de funciones del cliente (rcftpclient)     
// $Revision$ */
// Aunque se permite la modificación de cualquier parte del código, se 
// recomienda modificar solamente este fichero y su fichero de cabeceras asociado.
//*********************************************************************************

//*************************************************************************
// INCLUDES                                                               
//*************************************************************************
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include "rcftp.h"
#include "rcftpclient.h" 
#include "multialarm.h" 
#include "vemision.h" 
#include "misfunciones.h"

//**************************************************************************
// VARIABLES GLOBALES
//**************************************************************************

char* autores="Autores: Pascual Albericio, Irene\nAutor: Solana Melero, Carlos";
//char* autores="Autor: Apellidos, Nombre\nAutor: Apellidos, Nombre" // dos autores

// variable para indicar si mostrar información extra durante la ejecución
// como la mayoría de las funciones necesitaran consultarla, la definimos global
extern char verb;

// variable externa que muestra el número de timeouts vencidos
// Uso: Comparar con otra variable inicializada a 0; si son distintas, tratar un timeout e incrementar en uno la otra variable
extern volatile const int timeouts_vencidos;

//**************************************************************************
// Obtiene la estructura de direcciones del servidor 
//**************************************************************************
struct addrinfo* obtener_struct_direccion(char *dir_servidor, char *servicio, char f_verbose){
 	struct addrinfo hints, *servinfo,*direccion;	   
  	int status; 
 	int numdir = 1; 
	memset(&hints, 0, sizeof hints);
	
	if (f_verbose==1){
		printf("Viendo los parámetros necesarios para crar el socket. \n");
		fflush(stdout);
	}

	hints.ai_family = AF_UNSPEC;
	if (f_verbose==1){
		switch (hints.ai_family){
			case AF_INET: printf("IPv4. \n"); break;
			case AF_INET6: printf("IPv6. \n"); break;
			case AF_UNSPEC: printf("IPv4 y IPv6. \n"); break;
			default: printf("no ip (%d). \n", hints.ai_family); 
			break;
		}
		fflush(stdout);
	}
	hints.ai_socktype = SOCK_DGRAM;//UDP
	if (f_verbose==1){
		printf("Tipo: \n");
		switch (hints.ai_socktype){
			case SOCK_STREAM: printf("flujo (TCP). \n"); break;
			case SOCK_DGRAM: printf("datagrama (UDP). \n"); break;
			default: printf("no convencional (%d). \n", hints.ai_socktype); 
			break;
		}
 		fflush(stdout);
 	}

	if (dir_servidor != NULL){

		if (f_verbose==1) printf("Nombre y dirección: %s\n", dir_servidor);
	}
	else{
		if (f_verbose==1){
			printf("Nombre y dirección local. \n");
		} 
		hints.ai_flags = AI_PASSIVE ;
	}

	if (f_verbose==1){
		printf(" Servicio y puerto: %s. \n", servicio);
	} 

	if (f_verbose==1){
		printf("Uso getaddrinfo() para ver la estructura. ");
		fflush(stdout);
	}

	status = getaddrinfo(dir_servidor, servicio, &hints, &servinfo);
	if (status != 0)
	{
		fprintf(stderr,"Error:getaddrinfo() fallido. \n");
		exit(1);
 	}
	if (f_verbose==1){
		printf("OK. \n");
	} 
	
	if (f_verbose==1){
		printf("Analizando estructura de direcciones devuelta. \n");
		direccion = servinfo;
		while (direccion != NULL){ 
			printf("Dirección %d:\n", numdir);
			printsockaddr((struct sockaddr_storage*) direccion->ai_addr);
			
			direccion = direccion->ai_next;
			numdir++;
		}
	}
	return servinfo;
}

//**************************************************************************
// Imprime una direccion 
//**************************************************************************
void printsockaddr(struct sockaddr_storage * saddr){
	struct sockaddr_in *saddr_ipv4; 
	struct sockaddr_in6 *saddr_ipv6; 
	void *addr; 
	char ipstr[INET6_ADDRSTRLEN]; 
	int port; 
	if (saddr == NULL){}
	else{
		printf("Familia de direcciones: ");
		fflush(stdout);
		if (saddr->ss_family == AF_INET){ 
			printf("IPv4. \n");
			saddr_ipv4 =(struct sockaddr_in*)saddr ;
			addr = &(saddr_ipv4->sin_addr);
			port = ntohs(saddr_ipv4->sin_port);
		}
 		else if (saddr->ss_family == AF_INET6){
			printf("IPv6. \n");
			saddr_ipv6 = (struct sockaddr_in6 *)saddr;
			addr = &(saddr_ipv6->sin6_addr);
			port = ntohs(saddr_ipv6->sin6_port);
 		}
 		else{
			fprintf(stderr, "Familia errónea. \n");
			exit(1);
		}
		inet_ntop(saddr->ss_family, addr, ipstr, sizeof ipstr);
		printf("Dirección: %s. \n", ipstr);
		printf("Puerto local: %d. \n", port);
	}
}


//**************************************************************************
// Configura el socket, devuelve el socket y servinfo 
//**************************************************************************
int initsocket(struct addrinfo *servinfo, char f_verbose){
	int sock;
	if (f_verbose==1){
		printf("Pruebo a crear el socket. ");
		fflush(stdout);
	}
	sock = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);
	if (sock < 0){
		perror("Error:socket fallido. ");	
	}
	else{ 
		if (f_verbose==1){
			printf("OK\n");
		}
	}
	if (sock < 0){
		perror("Error: comunicación fallida. ");
		exit(1);
	}
	return sock;
}



//**************************************************************************
// esLaRespuestaEsperada(msgEnviado) rellena los campos del mensaje a enviar 
//**************************************************************************
int esLaRespuestaEsperada(struct rcftp_msg* msgRecibido,struct rcftp_msg* msgEnviado, int numSeqTotal,int numSeqMasViejo,int respuestaNext, int id){
	int esperado=1;
	if (msgRecibido->flags == F_BUSY ) { 
		esperado=0;
		fprintf(stderr,"Error: servidor ocupado. \n");
	}
	if (msgRecibido->flags == F_ABORT ) { 
		esperado=0;
		fprintf(stderr,"Error: finalizacion forzosa. \n");
	}
	if(id==0){
		if (htonl(msgRecibido->next) != (htonl(msgEnviado->numseq)+htons(msgEnviado->len))) { 
			esperado=0;
			fprintf(stderr,"Error: mensaje no esperado. \n"); 
		}
	}
	else if(id==1){
		if(respuestaNext-1 < numSeqMasViejo || respuestaNext-1 > numSeqTotal) { 
			esperado=0;
			fprintf(stderr,"Error: respuesta no aceptada en la ventana de emisión. \n"); 
		}
	}
	if ((msgEnviado->flags == F_FIN) && (msgRecibido->flags != F_FIN) && (id == 0)) { 
		esperado=0;
		fprintf(stderr,"Error: flags recibidos deberian ser F_FIN. \n");
	}
	return esperado; 
}


//**************************************************************************
// esMensajeValido(msgEnviado) rellena los campos del mensaje a enviar 
//**************************************************************************
int esMensajeValido(struct rcftp_msg* msgRecibido){
	int esperado=1;
	if (msgRecibido->version!=RCFTP_VERSION_1) {
		esperado=0;
		fprintf(stderr,"Error: mensaje con versión incorrecta. \n");
	}
	if (issumvalid((void *)msgRecibido,sizeof(*msgRecibido))==0) {
		esperado=0;
		fprintf(stderr,"Error: mensaje con checksum incorrecto. \n"); 
	}
	return esperado;
}


//**************************************************************************
// algoritmo 1 (basico) 
//**************************************************************************
void alg_basico(int socket, struct addrinfo *servinfo) {
	printf("Comunicación con algoritmo básico\n"); // Mensaje informativo

 	//reserva memoria para estructuras de mensajes enviados y recibidos
	struct rcftp_msg* msgEnviado = malloc(sizeof(struct rcftp_msg));
	struct rcftp_msg* msgRecibido = malloc(sizeof(struct rcftp_msg)); 

	ssize_t  send_bytes; //variables usadas para guardar el numero de bytes obtenidos al realizar funciones como sendto o recvfrom
	ssize_t  recv_bytes;
	
	int ultimoMensaje = 0;
	int ultimoMensajeConfirmado = 0;

	ssize_t numDatos;

	// Lee datos desde la entrada estándar y los almacena en el buffer del mensaje enviado
	numDatos = readtobuffer((char *)msgEnviado->buffer,RCFTP_BUFLEN); 

	
	if(numDatos <= 0){ // Si no hay más datos, marca el último mensaje
		ultimoMensaje = 1; 
	}

    // Configura los campos del mensaje enviado
	msgEnviado->sum=0;
	msgEnviado->version=RCFTP_VERSION_1;
	if(ultimoMensaje==1){
		msgEnviado->flags = F_FIN;
	}else{
		msgEnviado->flags=F_NOFLAGS;
	}
	msgEnviado->numseq=htonl(0);
	msgEnviado->next=htonl(0);
	msgEnviado->len=htons(numDatos);
	msgEnviado->sum= xsum((char*)msgEnviado,sizeof(*msgEnviado));

    // Bucle principal hasta que se confirma el último mensaje
	while(ultimoMensajeConfirmado == 0){
		send_bytes=sendto(socket, (char *)msgEnviado, sizeof(*msgEnviado), 0, servinfo->ai_addr, servinfo->ai_addrlen);
		if(send_bytes != sizeof(*msgEnviado)){
			if(send_bytes == -1){
				perror("Error: sendto fallido. \n");
			}
			else{
				fprintf(stderr, "Error: bytes del mensaje enviados incorrectamente. \n");
				exit(1);
			}
		}

		// Recibe la respuesta del servidor
		recv_bytes=recvfrom(socket, (char *)msgRecibido, sizeof(*msgRecibido), 0, servinfo->ai_addr, &(servinfo->ai_addrlen));
		if(recv_bytes < 0){
			perror("Error: recvfrom fallido. \n");
			exit(1);
		}

        // Verifica si la respuesta es la esperada y si el mensaje recibido es válido
		if(esLaRespuestaEsperada(msgRecibido,msgEnviado,0,0,0,0) && esMensajeValido(msgRecibido)){
			if(ultimoMensaje == 1){
				ultimoMensajeConfirmado = 1; 
			}else{
				// Lee más datos desde la entrada estándar y configura el siguiente mensaje
				numDatos = readtobuffer((char *)msgEnviado->buffer,RCFTP_BUFLEN); 
				if (numDatos <= 0){ // Si es el último mensaje, marca como confirmado
					ultimoMensaje = 1; 
				}
				
				// Configura los campos del mensaje enviado
				msgEnviado->sum=0;
				msgEnviado->version=RCFTP_VERSION_1;
				if(ultimoMensaje == 1){
					msgEnviado->flags= F_FIN;
				}
				else{
					msgEnviado->flags= F_NOFLAGS;
				}
								msgEnviado->numseq= msgRecibido->next;
				msgEnviado->next=htonl(0);
				msgEnviado->len=htons(numDatos);
				msgEnviado->sum= xsum((char*)msgEnviado,sizeof(*msgEnviado));
			}
		}
	}
}


//**************************************************************************
// algoritmo 2 (stop & wait) 
//**************************************************************************
void alg_stopwait(int socket, struct addrinfo *servinfo){
	printf("Comunicación con algoritmo stop&wait\n"); // Mensaje informativo

	//reserva memoria para estructuras de mensajes enviados y recibidos
	struct rcftp_msg* msgEnviado = malloc(sizeof(struct rcftp_msg)); 
	struct rcftp_msg* msgRecibido = malloc(sizeof(struct rcftp_msg)); 

	ssize_t send_bytes;
	ssize_t recv_bytes; 

	int ultimoMensaje = 0;
	int ultimoMensajeConfirmado = 0;

	ssize_t numDatos;
	int flagsS; 

    // Configuración de la señal SIGALRM para manejar las interrupciones del temporizador
	signal(SIGALRM,handle_sigalrm); 

    // Configuración del socket para operar en modo no bloqueante
	flagsS = fcntl(socket,F_GETFL,0);
	fcntl(socket,F_SETFL,flagsS | O_NONBLOCK);

    // Lectura de datos desde la entrada estándar y almacenamiento en el buffer del mensaje enviado
	numDatos = readtobuffer((char *)msgEnviado->buffer,RCFTP_BUFLEN);
	if(numDatos <= 0){ // Si no hay más datos, marca el último mensaje
		ultimoMensaje = 1;
	}

    // Configuración de los campos del mensaje enviado
	msgEnviado->version=RCFTP_VERSION_1;
	msgEnviado->sum=0;
	msgEnviado->numseq=htonl(0);
	if(ultimoMensaje==1){
		msgEnviado->flags = F_FIN;
	}else{
		msgEnviado->flags=F_NOFLAGS;
	}
	msgEnviado->next=htonl(0);
	msgEnviado->len=htons(numDatos);
	msgEnviado->sum= xsum((char*)msgEnviado,sizeof(*msgEnviado));

    // Bucle principal hasta que se confirma el último mensaje
	while(ultimoMensajeConfirmado == 0){
	    // Envío del mensaje al servidor
		send_bytes=sendto(socket, (char *)msgEnviado, sizeof(*msgEnviado), 0, servinfo->ai_addr, servinfo->ai_addrlen);
		if(send_bytes != sizeof(*msgEnviado)){
			if(send_bytes == -1){
				perror("Error: sendto fallido. \n");
			}
			else{
				fprintf(stderr, "Error: bytes del mensaje enviados incorrectamente. \n");
			}
			exit(1);
		}

        // Manejo de temporizador y recepción de la respuesta del servidor
		if(addtimeout()){
			int timeoutsProcesados = 0;
			int wait = 1;
			while(wait == 1){ // Bucle hasta que se recibe la respuesta del servidor o se agotan los timeouts
				recv_bytes = recvfrom(socket,(char *)msgRecibido, sizeof(*msgRecibido), 0, servinfo->ai_addr, &(servinfo->ai_addrlen)); // Recepción de datos desde el servidor
				if(recv_bytes < 0 && errno != EAGAIN){ // Verificación de errores en la recepción
					perror("Error: recvfrom fallido. \n");
					exit(1);
				}
				if(recv_bytes > 0){ // Si se reciben datos, se cancela el temporizador y se sale del bucle
					canceltimeout();
					wait = 0;
				}
				if(timeoutsProcesados != timeouts_vencidos){ // Verificación si se han agotado los timeouts
					wait = 0;
					timeoutsProcesados++ ;
				} 
			}
		}

		// Verificación de la respuesta recibida y del mensaje válido
		if(esLaRespuestaEsperada(msgRecibido,msgEnviado,0,0,0,0) && esMensajeValido(msgRecibido)){
			if(ultimoMensaje == 1){ // Si es el último mensaje, marca como confirmado
				ultimoMensajeConfirmado = 1; 
			}
			else{
				// Lectura de más datos desde la entrada estándar y configuración del siguiente mensaje
				numDatos = readtobuffer((char *)msgEnviado->buffer,RCFTP_BUFLEN);

				if(numDatos <= 0){ // Si no hay más datos, marca el último mensaje
					ultimoMensaje = 1; 
				}

    			// Configuración de los campos del mensaje enviado
				msgEnviado->version=RCFTP_VERSION_1;
				msgEnviado->sum=0;
				msgEnviado->numseq=msgRecibido->next;
				if(ultimoMensaje==1){
					msgEnviado->flags = F_FIN;
				}else{
					msgEnviado->flags=F_NOFLAGS;
				}
				msgEnviado->next=htonl(0);
				msgEnviado->len=htons(numDatos);
				msgEnviado->sum= xsum((char*)msgEnviado,sizeof(*msgEnviado));	
			}
		}
	}
}


//**************************************************************************
//* algoritmo 3 (ventana deslizante)
//**************************************************************************
void alg_ventana(int socket, struct addrinfo *servinfo,int window) {
    printf("Comunicación con algoritmo go-back-n\n"); // Mensaje informativo

	//reserva memoria para estructuras de mensajes enviados y recibidos
    struct rcftp_msg *msgEnviado = malloc(sizeof(struct rcftp_msg)); 
    struct rcftp_msg *msgRecibido = malloc(sizeof(struct rcftp_msg)); 
    ssize_t send_bytes;
    ssize_t recv_bytes; 

    int ultimoMensaje = 0;

    ssize_t numDatos;

    ssize_t sizeDatosVentana; 
    ssize_t numSeq;
    int respuestaNext;
    int numSeqMasViejo = 0;
    int numSeqTotal = 0;
	int timeoutsProcesados = 0;
	
	// Configuración de la señal SIGALRM para manejar las interrupciones del temporizador
    signal(SIGALRM, handle_sigalrm); 

    // Configuración del socket para operar en modo no bloqueante
    int flagsS; 
    flagsS = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flagsS | O_NONBLOCK);

    // Configuración del tamaño de la ventana
    setwindowsize(window);

    // Bucle principal hasta que se envíe el último mensaje
    while (ultimoMensaje == 0) {
        //envío
        if (getfreespace() >= RCFTP_BUFLEN && ultimoMensaje == 0) {
            // Lectura de datos desde la entrada estándar y almacenamiento en el buffer del mensaje enviado
			numDatos = readtobuffer((char *)msgEnviado->buffer, RCFTP_BUFLEN);
            if (numDatos <= 0){
                ultimoMensaje = 1;
				msgEnviado->flags = F_FIN;
            }
			
			else{
				msgEnviado->flags=F_NOFLAGS;
			}

            // Configuración de los campos del mensaje enviado
			msgEnviado->version=RCFTP_VERSION_1;
			msgEnviado->sum=0;
			msgEnviado->numseq=htonl(numSeqTotal);
			msgEnviado->next=htonl(0);
			msgEnviado->len=htons(numDatos);
			msgEnviado->sum= xsum((char*)msgEnviado,sizeof(*msgEnviado));			

            // Actualización del contador total de secuencias
            numSeqTotal = numSeqTotal + numDatos;
            
			// Envío del mensaje al servidor
            send_bytes = sendto(socket, (char *)msgEnviado, sizeof(*msgEnviado), 0, servinfo->ai_addr, servinfo->ai_addrlen);
            if (send_bytes != sizeof(*msgEnviado)) {
                if (send_bytes != -1) {
                    fprintf(stderr, "Error: enviados bytes de un mensaje incorrectos. \n");
                } else {
                    perror("Error: sendto incorrecto. \n");
                    exit(1);
                }
            }

            // Configuración del temporizador y actualización de la ventana de emisión
            addtimeout();
            sizeDatosVentana = addsentdatatowindow((char *)msgEnviado->buffer, numDatos);
            if (sizeDatosVentana != numDatos) {
                fprintf(stderr, "Error: añadidos a la ventana unos bytes incorrectos. \n");
            }

        }

        // recepción
        recv_bytes = recvfrom(socket, (char *)msgRecibido, sizeof(*msgRecibido), 0, servinfo->ai_addr, &(servinfo->ai_addrlen));
        if (recv_bytes > 0) {
            respuestaNext = ntohl(msgRecibido->next); // Extracción de información del mensaje recibido

            // Verificación de la validez del mensaje recibido y si es la respuesta esperada
            if (esMensajeValido(msgRecibido) && esLaRespuestaEsperada(msgRecibido, msgRecibido, numSeqTotal, numSeqMasViejo, respuestaNext, 1)) {
                printf("Mensaje OK. \n");
			    
				// Cancelación del temporizador, actualización de la ventana y del número de secuencia más viejo
                canceltimeout();
                printvemision();
                freewindow(respuestaNext);
                numSeqMasViejo = respuestaNext;
                printf("Ventana OK. \n");
                printvemision();
            }
        }

        //timeout
        if (timeoutsProcesados != timeouts_vencidos) {
            printf("Faltan mensajes que reenviar. \n");

            int length = RCFTP_BUFLEN; // Variables para almacenar la longitud y el número de secuencia del mensaje a reenviar
            numSeq = getdatatoresend((char *)msgEnviado->buffer, &length);
            printf("Mensaje reenviado con longitud: %d y num seq: %d. \n", length, (int)numSeq);
			
			// Configuración de los campos del mensaje a reenviar
			msgEnviado->version=RCFTP_VERSION_1;
			msgEnviado->sum=0;
			msgEnviado->numseq=htonl(numSeq);
			if(ultimoMensaje == 1){
				msgEnviado->flags = F_FIN;
			}else{
				msgEnviado->flags=F_NOFLAGS;
			}
			msgEnviado->next=htonl(0);
			msgEnviado->len=htons(length);
			msgEnviado->sum= xsum((char*)msgEnviado,sizeof(*msgEnviado));	

            // Envío del mensaje al servidor
		    send_bytes = sendto(socket, (char *)msgEnviado, sizeof(*msgEnviado), 0, servinfo->ai_addr, servinfo->ai_addrlen);
            if (send_bytes != sizeof(*msgEnviado)) {
                if (send_bytes != -1) { 
                    fprintf(stderr, "Error: enviados bytes de un mensaje incorrectos. \n");
                } else {
                    perror("Error: sendto incorrecto. \n");

                    exit(1);
                }
            }

            // Configuración del temporizador y actualización del contador de timeouts procesados
            addtimeout();
            timeoutsProcesados++;
        }
    }
}