/*
 * File: Almacen.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include <vector>
#include <iostream>
using namespace std;

template <typename T>
class Almacen 
{
    protected:
        vector<T*> _items; // Vector que almacena los elementos de tipo T.
        double _capacidad; // Capacidad máxima del almacén.

    public:
        // Constructor de la clase Almacen. Inicializa la capacidad máxima del almacén.
        Almacen(const double& capacidad)
            : _capacidad(capacidad)
        {}

        // Método para obtener la capacidad máxima del almacén.
        double capacidad() const {
            return _capacidad;
        }

        // Método para guardar un elemento en el almacén.
        // Devuelve true si se pudo guardar correctamente, false si no hay suficiente espacio.
        bool guardar(T* elemento) {
            double capacidadNew = 0.0;
            // Calcula el volumen total actual de los elementos en el almacén.
            for (auto i : _items) {
                capacidadNew = capacidadNew + i->volumen();
            }
            capacidadNew = capacidadNew + elemento->volumen();

            // Comprueba si se excede la capacidad máxima del almacén.
            if (capacidadNew <= _capacidad) {
                _items.push_back(elemento); // Agrega el elemento al vector de items.
                return true; // Se pudo guardar el elemento correctamente.
            }
            else {
                return false; // No hay suficiente espacio para guardar el elemento.
            }
        }
};