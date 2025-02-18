//-------------------------------------------------------------------------------------------
// File:   actividad.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------

#ifndef ACTIVIDAD_HPP
#define ACTIVIDAD_HPP

#include <iostream>
using namespace std;

// PREDECLARACIÓN DEL TAD 
struct actividad;
void crearActividad(string nombre, string descripcion, int tEstimado, actividad &a);
string obtenerNombre(const actividad &a);
string obtenerDescripcion(const actividad &a);
int obtenerTiempoEstimado(const actividad &a);

struct actividad {
    friend void crearActividad(string nombre, string descripcion, int tEstimado, actividad &a);
    friend string obtenerNombre(const actividad &a);
    friend string obtenerDescripcion(const actividad &a);
    friend int obtenerTiempoEstimado(const actividad &a);

    private:
    string nombre;
    string descripcion;
    int tEstimado;
};
#endif 