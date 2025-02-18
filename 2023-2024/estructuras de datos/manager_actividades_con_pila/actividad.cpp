//-------------------------------------------------------------------------------------------
// File:   actividad.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------
#include "actividad.hpp"

//Pre: ...
//Post: devuelve una actividad a partir de los datos nombre, descripcion y tEstimado
void crearActividad(string nombre, string descripcion, int tEstimado, actividad &a){
    a.nombre=nombre;
    a.descripcion=descripcion;
    a.tEstimado=tEstimado;
}

//Pre: ...
//Post: devuelve el nombre de la actividad a
string obtenerNombre(const actividad &a){
    return a.nombre;
}

//Pre: ...
//Post: devuelve la descripción de la actividad a
string obtenerDescripcion(const actividad &a){
    return a.descripcion;
}

//Pre: ...
//Post: devuelve el tiempo estimado de la actividad a
int obtenerTiempoEstimado(const actividad &a){
    return a.tEstimado;
}

