/*
 * File: Camion.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include "General.h"
#include "Almacen.h"
#include "Carga.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Camion : public General, public Almacen<Carga> 
{
    public:
        // Constructor de la clase Camion. Inicializa tanto la clase base General como la clase base Almacen<Carga>.
        Camion(double capacidad) 
            : General("Camion", capacidad, 0.0), Almacen(capacidad)
        {}

        // Método para mostrar información sobre el camión y sus elementos de carga.
        string mostrar(const string& sep) const override {
            stringstream ss;
            // Agrega información sobre el camión (nombre, volumen, peso) al stringstream.
            ss << sep << _nombre << " [" << volumen() << " m3] [" << peso() << " kg]\n";
            // Itera sobre los elementos de carga almacenados en el camión y agrega su información al stringstream.
            for (auto i : _items) {
                ss << i->mostrar(sep + "  ");
            }
            return ss.str(); // Devuelve la cadena construida con la información del camión y sus elementos de carga.
        }

        // Método para calcular el peso total del camión, sumando los pesos de todos sus elementos de carga.
        double peso() const override {
            double aux = 0.0;
            // Itera sobre los elementos de carga almacenados en el camión y suma sus pesos.
            for (auto i : this->_items) {
                aux += i->peso();
            }
            return aux; // Devuelve el peso total del camión.
        }

        // Sobrecarga del operador de inserción para imprimir un Camion en un flujo de salida.
        friend ostream& operator<<(ostream& os, const Camion& a) {
            os << a.mostrar(""); // Imprime la información del camión utilizando la función mostrar().
            return os; // Devuelve el flujo de salida.
        }
};