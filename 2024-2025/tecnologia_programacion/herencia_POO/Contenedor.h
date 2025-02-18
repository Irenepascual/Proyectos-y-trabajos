/*
 * File: Contenedor.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include "Carga.h"
#include "Almacen.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

template <class U>
class Contenedor : public Carga, public Almacen<U>
{
    public:
        // Constructor de la clase Contenedor. Inicializa tanto la clase base Carga como la clase base Almacen<U>.
        Contenedor(double capacidad) 
            : Carga("Contenedor", capacidad, 0.0), Almacen<U>(capacidad)
        {}
        
        // Método para mostrar información sobre el contenedor y sus elementos.
        string mostrar(const string& sep) const override {
            stringstream ss;
            // Agrega información sobre el contenedor (nombre, volumen, peso) al stringstream.
            ss << sep << _nombre << " [" << volumen() << " m3] [" << peso() << " kg]" << Almacen<U>::_items.back()->tipo() << "\n";
            // Itera sobre los elementos almacenados en el contenedor y agrega su información al stringstream.
            for (auto i : this->_items) {
                ss << i->mostrar(sep + "  ");
            }
            return ss.str(); // Devuelve la cadena construida con la información del contenedor y sus elementos.
        }

        // Método para calcular el peso total del contenedor, sumando los pesos de todos sus elementos.
        double peso() const override {
            double aux = 0.0;
            // Itera sobre los elementos almacenados en el contenedor y suma sus pesos.
            for (auto i : this->_items) {
                aux += i->peso();
            }
            return aux; // Devuelve el peso total del contenedor.
        }

        // Sobrecarga del operador de inserción para imprimir un Contenedor en un flujo de salida.
        friend ostream& operator<<(ostream& os, const Contenedor* a) {
            os << a->mostrar(""); // Imprime la información del contenedor utilizando la función mostrar().
            return os; // Devuelve el flujo de salida.
        }
};