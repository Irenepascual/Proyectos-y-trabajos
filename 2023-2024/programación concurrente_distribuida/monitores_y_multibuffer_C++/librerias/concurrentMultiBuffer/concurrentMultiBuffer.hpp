//-------------------------------------------------------------------------------------------
// File:   concurrentBuffer.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
// Coms:   Este programa usa monitores para enviar las tareas mediante el buffer, recoger esas tareas desde el controlador, ejecutar las tareas, 
//         enviar la tarea final y mostrar la matriz de resultados. 
//-------------------------------------------------------------------------------------------

#ifndef CONCURRENTMULTIBUFFER_HPP
#define CONCURRENTMULTIBUFFER_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "MultiBuffer.hpp"

using namespace std;

template <typename T,unsigned N>            //Declaramos el monitor, con sus operaciones y sus condiciones
class concurrentMultiBuffer{
public:
    void putTareas(const T tarea);          //Operaciones
    T getTarea(const unsigned pos);
    void putTareaFinal(const T tarea);

private:
    mutex mutexconcurrentMultibuffer;       //mutex
    MultiBuffer<T,N> mBT;
    condition_variable C_cogerTarea[N];     //condiciones para sincronizacion entre las operaciones
    condition_variable C_bufferLibre;
    condition_variable C_todosBufferLibres;
};


#endif                                      //A partir de aquí comienza el código de las operaciones

//Operacion de poner tareas en el buffer
template <typename T,unsigned N>
void concurrentMultiBuffer<T,N>::putTareas(const T tarea) {     
    unique_lock<mutex> lck(mutexconcurrentMultibuffer);
    while(mBT.numberNews() >= mBT.size()) {                     //Esperar hasta que haya algún buffer libre
        C_bufferLibre.wait(lck);                                //Si no hay ningún buffer libre, se queda bloqueado hasta que otra operación lo desbloquee
    }
    unsigned pos = 0;
    bool terminar = false;
    while(!terminar) {
        if(!mBT.isNew(pos)) {                                   //Busca la posición del buffer libre
            mBT.put(tarea, pos);                                //Colocamos la tarea en un buffer libre
            C_cogerTarea[pos].notify_all();                     //Desbloquea a la condición de coger tareas (solo desbloquea del de la posición con el buffer al 
            terminar = true;                                    //que lo envia), para que la recoja
        }
        pos++;                                                  //Si no ha encontrado buffer libre, va aumentando la posición para ver si el siguiente está libre
    }
}

//Operacion de recoger tareas del buffer
template <typename T,unsigned N>                                
T concurrentMultiBuffer<T,N>::getTarea(const unsigned pos) {
    unique_lock<mutex> lck(mutexconcurrentMultibuffer);
    while(!mBT.isNew(pos)) {                                    //Esperar hasta que llegue la tarea
        C_cogerTarea[pos].wait(lck);                            //Desbloquea solo la condición de la posición cuyo buffer esta libre
    }
    T tarea = mBT.get(pos);                                     //guarda la tarea recogida en el struct tarea
    C_todosBufferLibres.notify_all();                           //Desbloquea esa condicion porque se ha quedado un buffer libre
    C_bufferLibre.notify_all();                                 //Desbloquea esa condicion porque se ha quedado un buffer libre
    return tarea;
}

//Operación para enviar la tarea final al buffer
template <typename T,unsigned N>
void concurrentMultiBuffer<T,N>::putTareaFinal(const T tarea) { 
    unique_lock<mutex> lck(mutexconcurrentMultibuffer);
    while(mBT.numberNews() > 0) {                               //Esperar hasta que todos los buffers estén libres
        C_todosBufferLibres.wait(lck);                          //sino se queda bloqueado
    }
    for(unsigned i = 0; i < mBT.size(); i++) {                  //Envia a cada buffer la tarea final
        mBT.put(tarea, i);                                      
        C_cogerTarea[i].notify_all();                           //Desbloquea la condicion de coger tarea de todos las posiciones
    }
}