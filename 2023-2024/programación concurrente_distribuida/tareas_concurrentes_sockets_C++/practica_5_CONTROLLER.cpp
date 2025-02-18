//-------------------------------------------------------------------------------------------
// File:   practica_5_CONTROLLER.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
// Coms:   Este programa se encarga de recibir las tareas y cargas de trabajo para ejecutarlas y enviarle
//         las anteriores informaciones más el hecho de si se ha ejecutado o no a MATRIX_SERVER.
//-------------------------------------------------------------------------------------------

#include <MultiBuffer.hpp>
#include <escribir_Matriz.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <sstream>
#include "Socket.hpp"

using namespace std;

const int N_CONTROLLERS = 10;
const unsigned tiposTarea = 3;

struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};



void leerYejecutar(const string IPServT,const int puertoServT, const string IPServM,const int puertoServM, escribir_Matriz<tiposTarea>& escribir_Matriz, tarea& p, int j){
    string MENS_FIN = "END";    //declaramos las variables
    string tareafinal="TF";
    string buffer;
    string OK_NOOK;
    bool ejecucion = true;
    int length = 100;
    tarea p3;
    int i = 0;
    
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket channelT(IPServT, puertoServT);
    Socket channelM(IPServM, puertoServM);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_T;
    int socket_M;
    do {
        // Conexión con el servidor TASK_SERVER
        socket_T = channelT.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_T == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_T == -1 && count < MAX_ATTEMPS);

    count=0;
    do {
        // Conexión con el servidor MATRIX_SERVER
        socket_M = channelM.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_M == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_M == -1 && count < MAX_ATTEMPS);

    //nos mantendremos en el bucle hasta que reciba el mensaje final
    bool seguir = true;
    while(seguir) {
        int send_bytes = channelT.Send(socket_T, "GET TASK");           //Envia a TASK_SERVER "GET_TASK" para indicarle que le mande una tarea
        if(send_bytes == -1) {
            cerr << channelT.error("Error al enviar datos");
            // Cerramos los sockets
            channelT.Close(socket_T);
            exit(1);
        }

        int rcv_bytes = channelT.Receive(socket_T, buffer, length);     //Recibe la tarea de TASK_SERVER
        if (rcv_bytes == -1) {
            cerr << channelT.error("Error al recibir datos");
            // Cerramos los sockets
            channelT.Close(socket_T);
        }

            if(buffer == tareafinal){                                   //Si la tarea recibida es la tarea final...                
                send_bytes = channelM.Send(socket_M, tareafinal);       //Enviamos a MATRIX_SERVER esa tarea final
                if(send_bytes == -1) {
                    cerr << channelM.error("Error al enviar datos");
                    // Cerramos los sockets
                    channelM.Close(socket_M);
                    exit(1);
                }
                while(seguir){                                                      //nos mantendremos en el bucle hasta que reciba el mensaje final
                    int rcv_bytes = channelM.Receive(socket_M, buffer, length);     
                    if (rcv_bytes == -1) {
                        cerr << channelM.error("Error al recibir datos");
                        // Cerramos los sockets
                        channelM.Close(socket_M);
                    }
                    else{   
                        seguir=false;                                               //cuando se reciba llegará a esta zona del código
                    }
                }
                send_bytes = channelT.Send(socket_T,"END");                         //entonces envia "END" a TASK_SERVER
                if(send_bytes == -1) {
                    cerr << channelT.error("Error al mandar mensaje. Adios.");
                }
            }
            else{
                string aux;                             //Desglosamos el mensaje que nos ha enviado TASK_SERVER en tipo de tarea y carga de tarea

                int indice=0;
                while (buffer[indice] != ',') {         //hasta que encuentre una coma será la tarea
                    aux+=buffer[indice];
                    indice++;
                }
                p3.tipoTarea=aux;
                aux="";
                indice++;
                while (indice != buffer.size()) {       //hasta que termine la longitud del mensaje será la carga
                    aux+=buffer[indice];
                    indice++;
                }
                p3.cargaDeTrabajo= stof(aux);

                srand(j + time(NULL) + i);              //calculamos el numero random
                double random = rand() % (100 + 1);
                i++;

                escribir_Matriz.ejecutar(p3.tipoTarea, p3.cargaDeTrabajo, random, N_CONTROLLERS, ejecucion);       //ejecutamos las tareas
                if(ejecucion){
                    OK_NOOK = "OK";         //si se han ejecutado
                }
                else{
                    OK_NOOK = "NO_OK";      //si no se han ejecutado
                }
                string tarea = p3.tipoTarea + ',' + OK_NOOK + ',' + to_string(p3.cargaDeTrabajo);       //preparamos el string que vamos a enviar a MATRIX_SERVER (--,--,--)
                int send_bytes = channelM.Send(socket_M, tarea);    //lo enviamos
            } 
    }
    channelT.Close();   //cerramos los dos sockets
    channelM.Close();
}

int main(int argc,char* argv[]) {
    escribir_Matriz<tiposTarea> escribir_Matriz;    //declaramos variables
    thread P[N_CONTROLLERS];   
    tarea p;

    // Dirección y puerto de TASK_SERVER y MATRIX_SERVER
    string IPServT = (argv[1]);
    int puertoServT = stoi(argv[2]);
    string IPServM = (argv[3]);
    int puertoServM = stoi(argv[4]);

    for(int i=0; i<N_CONTROLLERS; i++){         //lanzamos N_CONTROLADORES clientes
        P[i] = thread(&leerYejecutar, IPServT, puertoServT, IPServM, puertoServM, ref(escribir_Matriz), ref(p), i);
    }
    
    for(int i=0; i<N_CONTROLLERS; i++){
        P[i].join();
    }

    return 0;
}
