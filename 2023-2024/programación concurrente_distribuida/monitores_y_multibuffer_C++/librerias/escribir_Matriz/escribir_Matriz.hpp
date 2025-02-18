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
    void escribe(string tarea, double cargaDeTrabajo, const int random,  int id);
    void mostrar_Matriz();
private:
    mutex mutexescribirMatriz;              //mutex
    int matriz[3][3];
    int numFinales=0;
    condition_variable C_desbloquear;       //condiciones para sincronizacion entre las operaciones
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

//Operacion para ejecutar las tareas en base al numero random
template <unsigned N>
void escribir_Matriz<N>::escribe(string tarea, double cargaDeTrabajo, const int random, int id){        
    unique_lock<mutex> lck(mutexescribirMatriz);          
    if(tarea == "TF"){                              //Si llega una tarea final
        numFinales++;                               //Aumenta la variable numFinales
        if(numFinales == 10){                       //Cuando esa variable sea igual a N_CONTROLLERS
            C_desbloquear.notify_all();             //Desbloquea poder mostrar la matriz
        }
    }
    if(tarea == "t1") {                             //si el tipo de la tarea es t1
        matriz[0][0]++;                             //Anotamos que ha llegado una tarea t1
        if (random<=95){                            //Si el numero random es mayor que el 5%, se ejecuta
            this_thread::sleep_for(chrono::milliseconds(int(cargaDeTrabajo)));  //se ejecuta
            matriz[0][2] = matriz[0][2] + cargaDeTrabajo;   //Sumamos el tiempo de carga de t1
            matriz[0][1]++;                         //Anotamos el numero de exitos de t1
        }
    }
    if(tarea == "t2") {                             //si el tipo de la tarea es t2
        matriz[1][0]++;                             //Anotamos que ha llegado una tarea t2
        if (random<=93){                            //Si el numero random es mayor que el 7%, se ejecuta
            this_thread::sleep_for(chrono::milliseconds(int(cargaDeTrabajo)));  //se ejecuta
            matriz[1][2] = matriz[1][2] + cargaDeTrabajo;   //Sumamos el tiempo de carga de t2
            matriz[1][1]++;                         //Anotamos el numero de exitos de t2
        }
    }
    if(tarea == "t3") {
        matriz[2][0]++;                             //si el tipo de la tarea es t3
        if (random<=90){                            //Anotamos que ha llegado una tarea t3
            this_thread::sleep_for(chrono::milliseconds(int(cargaDeTrabajo)));  //se ejecuta
            matriz[2][2] = matriz[2][2] + cargaDeTrabajo;   //Sumamos el tiempo de carga de t3
            matriz[2][1]++;                         //Anotamos el numero de exitos de t3
        }
    }
}

//Operacion que muestra por pantalla los resultados obtenidos
template <unsigned N>
void escribir_Matriz<N>::mostrar_Matriz(){          
    unique_lock<mutex> lck(mutexescribirMatriz);     
    while(numFinales!=10){    
        C_desbloquear.wait(lck);                //Se bloquea hasta que llegan todas las tareas finales
    }                
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