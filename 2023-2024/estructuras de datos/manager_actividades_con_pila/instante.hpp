//-------------------------------------------------------------------------------------------
// File:   instante.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------
#ifndef INSTANTE_HPP
#define INSTANTE_HPP

#include <iostream>
using namespace std;

// PREDECLARACIÓN DEL TAD 
struct instante;
bool crearInstante(const int dia, const int mes, const int anyo, const int hora, const int minuto, instante &i);
int dia(const instante &i);
int mes(const instante &i);
int anyo(const instante &i);
int hora(const instante &i);
int minuto(const instante &i);
bool operator==(const instante &i1, const instante &i2);
bool anterior1(const instante &i1, const instante &i2);
bool posterior1(const instante &i1, const instante &i2);
int diferencia(const instante &i1, const  instante &i2);

struct instante {
  friend bool crearInstante(const int dia, const int mes, const int anyo, const int hora, const int minuto, instante &i);
  friend int dia(const instante &i);
  friend int mes(const instante &i);
  friend int anyo(const instante &i);
  friend int hora(const instante &i);
  friend int minuto(const instante &i);
  friend bool operator==(const instante &i1, const  instante &i2);
  friend bool anterior1(const instante &i1, const  instante &i2);
  friend bool posterior1(const instante &i1, const  instante &i2);
  friend int diferencia(const instante &i1, const  instante &i2);

  private: 
    int minuto;
    int hora;
    int dia;
    int mes;
    int anyo;
};
#endif 