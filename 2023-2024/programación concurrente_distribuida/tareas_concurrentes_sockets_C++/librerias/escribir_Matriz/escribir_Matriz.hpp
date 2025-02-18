//-------------------------------------------------------------------------------------------
// File:   escribir_Matriz.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
// Coms:   Este programa usa monitores para enviar las tareas mediante el buffer, recoger esas tareas desde el controlador, ejecutar las tareas, 
//         enviar la tarea final y mostrar la matriz de resultados. 
//-------------------------------------------------------------------------------------------

#ifndef ESCRIBIR_MATRIZ_HPP
#define ESCRIBIR_MATRIZ_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <iomanip>  
#include <condition_variable>


using namespace std;

template <unsigned N>
class escribir_Matriz{                      //Declaramos el monitor, con sus operaciones y sus condiciones
public:                                     //Operaciones
    escribir_Matriz();
    void ejecutar(string tarea, float cargaDeTrabajo, double random, const int N_CONTROLLERS, bool& ejecucion);                      
    void escribe(string tarea, float cargaDeTrabajo, string ejecutado, const int N_CONTROLLERS);
    void mostrar_Matriz(const int N_CONTROLLERS);
private:
    int matriz[3][3];
};


#endif

template <unsigned N>
escribir_Matriz<N>::escribir_Matriz(){      //Inicializamos la matriz a 0
    for(unsigned i = 0; i < N; i++) {
        for(unsigned j = 0; j < N; j++) {
            matriz[i][j] = 0;
        }
    }
}

template <unsigned N>
void escribir_Matriz<N>::ejecutar(string tarea, float cargaDeTrabajo, double random, const int N_CONTROLLERS, bool& ejecucion){
    if(tarea == "t1") {                             //si el tipo de la tarea es t1
        if (random<=95){                            //Si el numero random es mayor que el 5%, se ejecuta
            this_thread::sleep_for(chrono::milliseconds(int(cargaDeTrabajo)));  //se ejecuta
            ejecucion=true;
        }
        else{
            ejecucion=false;
        }
    }
    if(tarea == "t2") {                             //si el tipo de la tarea es t2
        if (random<=93){                            //Si el numero random es mayor que el 7%, se ejecuta
            this_thread::sleep_for(chrono::milliseconds(int(cargaDeTrabajo)));  //se ejecuta
            ejecucion=true;
        }
        else{
            ejecucion=false;
        }
    }
    if(tarea == "t3") {
        if (random<=90){                            //Anotamos que ha llegado una tarea t3
            this_thread::sleep_for(chrono::milliseconds(int(cargaDeTrabajo)));  //se ejecuta
            ejecucion=true;
        }
        else{
            ejecucion=false;
        }
    }
}


//Operacion para ejecutar las tareas en base al numero random
template <unsigned N>
void escribir_Matriz<N>::escribe(string tarea, float cargaDeTrabajo, string ejecutado, int N_CONTROLLERS){        
    if(tarea == "t1") {                             //si el tipo de la tarea es t1
        matriz[0][0]++;                             //Anotamos que ha llegado una tarea t1
        if (ejecutado=="OK"){                            //Si el numero random es mayor que el 5%, se ejecuta
            matriz[0][2] = matriz[0][2] + cargaDeTrabajo;   //Sumamos el tiempo de carga de t1
            matriz[0][1]++;                         //Anotamos el numero de exitos de t1
        }

    }
    if(tarea == "t2") {                             //si el tipo de la tarea es t2
        matriz[1][0]++;                             //Anotamos que ha llegado una tarea t2
        if (ejecutado=="OK"){                            //Si el numero random es mayor que el 7%, se ejecuta
            matriz[1][2] = matriz[1][2] + cargaDeTrabajo;   //Sumamos el tiempo de carga de t2
            matriz[1][1]++;                         //Anotamos el numero de exitos de t2
        }
    }
    if(tarea == "t3") {
        matriz[2][0]++;                             //si el tipo de la tarea es t3
        if (ejecutado=="OK"){                            //Anotamos que ha llegado una tarea t3
            matriz[2][2] = matriz[2][2] + cargaDeTrabajo;   //Sumamos el tiempo de carga de t3
            matriz[2][1]++;                         //Anotamos el numero de exitos de t3
        }
    }
}

//Operacion que muestra por pantalla los resultados obtenidos
template <unsigned N>
void escribir_Matriz<N>::mostrar_Matriz(int N_CONTROLLERS){                
    cout << endl;
    for (int i = 0; i < 3; i++){                //Muestra la matriz por pantalla
        for (int j = 0; j < 3; j++)
        {
            cout<< left << setw(6) << matriz[i][j];
        }
        cout << endl;
    }
    cout << endl;
}