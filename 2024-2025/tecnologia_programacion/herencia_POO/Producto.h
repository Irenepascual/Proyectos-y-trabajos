/*
 * File: Producto.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include "Carga.h"
#include <iostream>
using namespace std;

class Producto : public Carga
{
    public:
        // Constructor de la clase Producto. Inicializa la clase base Carga con los valores proporcionados.
        Producto(const string& nombre, double volumen, double peso)
            : Carga(nombre, volumen, peso)
        {}
};