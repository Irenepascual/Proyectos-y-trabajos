/*
 * File: CargasEspec.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include "General.h"
#include <iostream>
using namespace std;

class SerVivo : public General 
{
    public:
        // Constructor de la clase SerVivo. Inicializa la clase base General con los valores proporcionados.
        SerVivo(const string& nombre, double volumen, double peso)
            : General(nombre, volumen, peso)
        {}
    
        // Sobrescribe el método tipo() de la clase base para proporcionar un tipo específico para los seres vivos.
        string tipo() const override
        {
            return " de Seres Vivos";
        }
};

class Toxico : public General 
{
    public:
        // Constructor de la clase Toxico. Inicializa la clase base General con los valores proporcionados.
        Toxico(const string& nombre, double volumen, double peso)
            : General(nombre, volumen, peso)
        {}

        // Sobrescribe el método tipo() de la clase base para proporcionar un tipo específico para productos tóxicos.
        string tipo() const override
        {
            return " de Productos Toxicos";
        }
};