//-------------------------------------------------------------------------------------------
// File:   practica_4.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar
// Coms:   Este programa usa monitores para enviar las tareas mediante el buffer, recoger esas tareas desde el controlador, ejecutar las tareas, 
//         enviar la tarea final y mostrar la matriz de resultados. 
//-------------------------------------------------------------------------------------------
#include <MultiBuffer.hpp>
#include <concurrentMultiBuffer.hpp>
#include <escribir_Matriz.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <iomanip>

using namespace std;

//Declaramos constantes
const int N_CONTROLLERS = 10;
const unsigned tiposTarea = 3;

//El registro de tarea
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};


//Pre: utilizamos el struct tarea y los monitores del concurrentBuffer y el de escribir_Matriz
//Post: lee del fichero las tareas y se las pasa al primer monitor para que ponga en el buffer la tarea y la mande. Una vez
//      han terminado todas las tareas del fichero, manda la tarea que indica la tarea final ("TF"). Cuando se han enviado todas 
//      las tareas finales, se muestra la matriz.
void masterTask(concurrentMultiBuffer<tarea,N_CONTROLLERS>& concurrentMultibuffer, escribir_Matriz<tiposTarea>& escribir_Matriz, tarea& p) {
    string nombre="tareas.txt";
    ifstream fich (nombre);
    string aux;
    string aux2;
    if (fich.is_open()){                            //Comprueba si el fichero está abierto
        while (!fich.eof()) {                       //Si no hay más tareas, pasa a escribir la tarea final
            getline(fich,aux,',');
            p.tipoTarea=aux;                        //Guarda el tipo de tarea en el struct tarea
            getline(fich,aux2,'\n');
            p.cargaDeTrabajo=stof(aux2);            //Guarda el tiempo de duracion de la tarea en el struct tarea
            concurrentMultibuffer.putTareas(p);     //Manda al buffer la tarea
        }
        p.tipoTarea = "TF";                         
        concurrentMultibuffer.putTareaFinal(p);     //Cuando termina de leer tareas, manda la tarea final al buffer
        escribir_Matriz.mostrar_Matriz();           //Mostramos la matriz con los resultados por la pantalla
        
        fich.close();                               //Una vez terminado, cerramos el fichero
    }
    else {
        cerr << "Error al abrir el fichero.\n";     //Si ha habido algún error al abrir el fichero
    }   
}

//Pre: 0 <= id < N_CONTROLLERS
//Pos: recoge la tarea de los buffers y se encarga de hacer que se ejecuten las tareas con un "tiempo aleatorio"
void controllerTask(unsigned int id, concurrentMultiBuffer<tarea,N_CONTROLLERS>& concurrentMultibuffer, escribir_Matriz<tiposTarea>& escribir_Matriz, tarea& p) {
    bool seguir = true;
    while (seguir) {
        tarea p2 = concurrentMultibuffer.getTarea(id);  //Obtiene la tarea enviada por el concurrentMultibuffer y la guarda en p2 (del struct tarea)
        if(p2.tipoTarea == "TF"){                   //Si la tarea recibida es la tarea final, deja de hacer el bucle.
            seguir = false;
        }
        srand(id + time(NULL));                     
        int random = rand() % (100 + 1);            //Genera un numero aleatorio entre 0 y 100
        escribir_Matriz.escribe(p2.tipoTarea, p2.cargaDeTrabajo, random, id);    //Enviamos los datos del struct, el proceso que toca y el numero random generado, para
                                                                                 //que nos ejecute la aleatoriedad de las tareas.
        
    }
}


int main(int argc, char *argv[]) {
    concurrentMultiBuffer<tarea,N_CONTROLLERS> concurrentMultibuffer;           //Declaramos
    escribir_Matriz<tiposTarea> escribir_Matriz;
    tarea p;
    thread P[N_CONTROLLERS + 1];                                                //Declaramos el thread
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        P[i] = thread(&controllerTask, i, ref(concurrentMultibuffer),ref(escribir_Matriz), ref(p));
    }
    P[N_CONTROLLERS] = thread(&masterTask, ref(concurrentMultibuffer),ref(escribir_Matriz), ref(p));
    for(unsigned i = 0; i <= N_CONTROLLERS; i++) {
        P[i].join();
    }
    return 0;
}
