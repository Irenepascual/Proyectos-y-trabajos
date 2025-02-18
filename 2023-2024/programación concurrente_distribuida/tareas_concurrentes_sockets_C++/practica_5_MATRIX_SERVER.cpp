//-------------------------------------------------------------------------------------------
// File:   practica_5_MATRIX_SERVER.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
// Coms:   Este programa se encarga de recibir los mensajes con las tareas, los datos de si se han ejecutado o no
//         y su carga de trabajo y lo va apuntando en la matriz hasta que estos procesos terminan (y muestra la matriz) 
//-------------------------------------------------------------------------------------------

#include <MultiBuffer.hpp>
#include <escribir_Matriz.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
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
const unsigned tiposTarea = 3;

struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};

void cogerTareas_Enviar(Socket& channel, int client_fd,escribir_Matriz<tiposTarea>& escribir_Matriz, tarea& p) {
    string MENS_FIN = "END";        //declaramos variables
    string ejecutado;
    string buffer;
    int length = 100;              
    tarea p4;

    bool seguir = true;             //nos mantenemos en el bucle hasta que llega una tarea final
    while(seguir) {
        // Recibimos el mensaje del cliente
        int rcv_bytes = channel.Receive(client_fd,buffer,length);
        if (rcv_bytes == -1) {
            cerr << channel.error("Error al recibir datos");
            // Cerramos los sockets
            channel.Close(client_fd);
        }
        else{
            if(buffer == "TF"){         //si ese mensaje recibido es una tarea final, nos salimos del bucle
                seguir=false;
            }
            else{                       //sino desglosamos el mensaje en tipo de tarea, si se ha ejecutado o no y carga de trabajo
                string aux;

                int indice=0;
                while (buffer[indice] != ',') {     //hasta una coma será el tipo de tarea
                    aux+=buffer[indice];
                    indice++;
                }
                p4.tipoTarea=aux;
                indice++;
                aux="";

                while (buffer[indice] != ',') {     //hasta la siguiente coma será si se ha ejecutado o no
                    aux+=buffer[indice];
                    indice++;
                }
                ejecutado=aux;
                indice++;

                aux="";
                while (indice < buffer.size()) {    //hasta la longitud del tamaño del mensaje, será la carga de trabajo
                    aux+=buffer[indice];
                    indice++;
                }
                p4.cargaDeTrabajo= stof(aux);

                escribir_Matriz.escribe(p4.tipoTarea, p4.cargaDeTrabajo, ejecutado, N_CONTROLLERS); //lo vamos escribiendo en la matriz
            }
        }
        
    }
    int send_bytes = channel.Send(client_fd, MENS_FIN);     //una vez ya ha llegado la tarea final, enviamos "END" a los controllers
    if(send_bytes == -1) {
        cerr << channel.error("Error al enviar datos");
        // Cerramos los sockets
        channel.Close(client_fd);
        exit(1);
    }
}


int main(int argc,char* argv[]) {
    escribir_Matriz<tiposTarea> escribir_Matriz;    //declaramos variables
    tarea p;
    thread P[N_CONTROLLERS + 1];  
    vector<thread> cliente;
    bool fin=false; 

    int SERVER_PORT = stoi(argv[1]);            //obtenemos el parámetro del puerto que nos dan y lo guardamos en esa variabl


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

    //aceptamos la conexión de N_CONTROLADORES y lanzamos N hilos 
    int clientes=0;
    while(clientes<N_CONTROLLERS){
        // Accept
        int new_client_fds = channel.Accept();

        if(new_client_fds == -1) {
            cerr << channel.error("Error en el accept");
            // Cerramos el socket
            channel.Close();
            exit(1);
        } 
        else {
            if(!fin){
                cliente.push_back(thread(&cogerTareas_Enviar, ref(channel), new_client_fds, ref(escribir_Matriz), ref(p)));
                clientes++;
            }
        }
    }

    
    for (int i=0; i<N_CONTROLLERS; i++) {
        cliente[i].join();          
    }
    //una vez ya se han ejecutado todos los hilos, mostramos la matriz
    escribir_Matriz.mostrar_Matriz(N_CONTROLLERS);          

    // Cerramos el socket del servidor
    error_code = channel.Close();
    if (error_code == -1) {
        cerr << channel.error("Error cerrando el socket del servidor");
    }

    return error_code;
}
