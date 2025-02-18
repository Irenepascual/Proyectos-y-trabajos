/*
 * File: Carga.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include "General.h"
#include <iostream>
using namespace std;

class Carga : public General
{
    public:
        // Constructor de la clase Carga. Inicializa la clase base General con los valores proporcionados.
        Carga(const string& nombre, double volumen, double peso)
            : General(nombre, volumen, peso)
        {}
};

