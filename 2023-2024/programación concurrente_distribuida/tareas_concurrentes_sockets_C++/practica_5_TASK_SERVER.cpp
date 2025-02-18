//-------------------------------------------------------------------------------------------
// File:   practica_5_TASK_SERVER.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
// Coms:   Este programa se encarga de leer las tareas del fichero y pasarselas al otro proceso para que
//         las vaya enviando a medida que le llegan peticiones para enviar tareas mediante "GET_TASK"
//-------------------------------------------------------------------------------------------

#include <MultiBuffer.hpp>
#include <concurrentMultiBuffer.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <vector>
#include "Socket.hpp"

using namespace std;

const int N_CONTROLLERS = 10;

struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};

void cogerTareas_Enviar(Socket& channel, int client_fd, int id, concurrentMultiBuffer<tarea,N_CONTROLLERS>& concurrentMultibuffer, tarea& p) {
    string MENS_FIN = "END";            //declaramos las variables
    int length = 100;
    string buffer;
    bool seguir = true;

    while(seguir) {
        // Recibimos el mensaje del cliente
        int rcv_bytes = channel.Receive(client_fd,buffer,length);

        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            cerr << channel.error("No puede llegar el END todavía");
            seguir = false; // Salir del bucle
        } else {
            tarea p2 = concurrentMultibuffer.getTarea(id);  //Obtiene la tarea enviada por el concurrentMultibuffer y la guarda en p2 (del struct tarea)
            if(p2.tipoTarea == "TF"){  
               //Si la tarea recibida es la tarea final, deja de hacer el bucle y envia la tarea final al controller
                seguir = false;
                int send_bytes = channel.Send(client_fd, p2.tipoTarea);
            }
            else{
                //si no es la tarea final, envia un string (--,--) al controller
                string tarea = p2.tipoTarea + ',' + to_string(p2.cargaDeTrabajo);
                int send_bytes = channel.Send(client_fd,tarea);
            }
        }
    }

    seguir = true; //volvemos a iniciar la variable a true para comenzar otro bucle
    while(seguir){
        int rcv_bytes = channel.Receive(client_fd,buffer,length); //se mantiene a la espera hasta recibir el mensaje final
        if (rcv_bytes == -1) {
            cerr << channel.error("Error al recibir datos");
            // Cerramos los sockets
            channel.Close(client_fd);
        }
        else if(buffer == MENS_FIN){                             //y cuando lo ha recibido, se sale del bucle
            seguir=false;
        }
    }
}

void leerTareas(concurrentMultiBuffer<tarea,N_CONTROLLERS>& concurrentMultibuffer, tarea& p) {
    string nombre="tareas.txt";     //declaramos las variables
    ifstream fich (nombre);
    string aux;
    string aux2;

    if (fich.is_open()){                            //Comprueba si el fichero está abierto
        while (getline(fich,aux,',')) {             //Si no hay más tareas, pasa a escribir la tarea final
            p.tipoTarea=aux;                        //Guarda el tipo de tarea en el struct tarea
            getline(fich,aux2,'\n');
            p.cargaDeTrabajo=stod(aux2);            //Guarda el tiempo de duracion de la tarea en el struct tarea
            concurrentMultibuffer.putTareas(p);     //Manda al buffer la tarea
        }
        p.tipoTarea = "TF";                         
        concurrentMultibuffer.putTareaFinal(p);     //Cuando termina de leer tareas, manda la tarea final al buffer        
        fich.close();                               //Una vez terminado, cerramos el fichero
    }
    else {
        cerr << "Error al abrir el fichero.\n";     //Si ha habido algún error al abrir el fichero
    }   
}


int main(int argc,char* argv[]) {
    concurrentMultiBuffer<tarea,N_CONTROLLERS> concurrentMultibuffer;   //declaramos las variables
    tarea p;
    thread P[N_CONTROLLERS + 1];   

    int SERVER_PORT = stoi(argv[1]);        //obtenemos el parámetro del puerto que nos dan y lo guardamos en esa variable

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket channel(SERVER_PORT);


    // Bind
    int socket_fd =channel.Bind();
    if (socket_fd == -1) {
        cerr << channel.error("Error en el bind");
        exit(1);
    }

    //listen
    int error_code = channel.Listen();
    if (error_code == -1) {
        cerr << channel.error("Error en el listen");
        // Cerramos el socket
        channel.Close();
        exit(1);
    }

    //Se mantendrá en el bucle hasta que no haya establecido conexión con todos los controllers
    int i=0;
    while(i < N_CONTROLLERS){
        // Accept
        int new_client_fds = channel.Accept();
        if(new_client_fds == -1) {
            cerr << channel.error("Error en el accept");
            // Cerramos el socket
            channel.Close();
            exit(1);
        } 
        else {
            P[i]=thread(&cogerTareas_Enviar, ref(channel), new_client_fds, i, ref(concurrentMultibuffer), ref(p));  //Envía N hilos a que lleven a cabo esta tarea
            i++;
        }
    }
    P[i] = thread(&leerTareas, ref(concurrentMultibuffer), ref(p)); //Envía un hilo a que lleve a cabo esta tarea

    //Espera a que todos los hilos terminen
    for (int i=0; i<=N_CONTROLLERS; i++) {
        P[i].join();
    }

    // Cerramos el socket del servidor
    error_code = channel.Close();
    if (error_code == -1) {
        cerr << channel.error("Error cerrando el socket del servidor");
    }

    return error_code;
}
