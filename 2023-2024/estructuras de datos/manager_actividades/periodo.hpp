//-------------------------------------------------------------------------------------------
// File:   periodo.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------

#ifndef PERIODO_HPP
#define PERIODO_HPP

#include "instante.hpp"
#include <iostream>
using namespace std;

// PREDECLARACIÓN DEL TAD 
struct periodo;
bool crearPeriodo(instante inicio,  instante final, periodo &p);
instante obtenerInicio(periodo &p);
instante obtenerFinal(periodo &p);
int obtenerDuracion(periodo &p);
bool iguales(const periodo &p1, const periodo &p2);
bool anterior(const periodo &p1,const periodo &p2);
bool solapados(const periodo &p1, const periodo &p2);


struct periodo {
    friend bool crearPeriodo(instante inicio,  instante final, periodo &p);
    friend instante obtenerInicio(periodo &p);
    friend instante obtenerFinal(periodo &p);
    friend int obtenerDuracion(periodo &p);
    friend bool iguales(const periodo &p1,const periodo &p2);
    friend bool anterior(const periodo &p1,const periodo &p2);
    friend bool solapados(const periodo &p1, const periodo &p2);

  private: 
    instante inicio;
    instante final;
};
#endif 