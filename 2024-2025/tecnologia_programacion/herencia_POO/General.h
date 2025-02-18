/*
 * File: General.h
 * Authors: Irene Pascual Albericio 871627 y Ariana Porroche Lloren 874055
 * Date: 13-03-2024
 */

#pragma once
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class General 
{
    protected:
        string _nombre;
        double _volumen;
        double _peso;

    public:
        // Constructor de la clase General. Inicializa los miembros de datos con los valores proporcionados.
        General(const string& nombre, double volumen, double peso)
            : _nombre(nombre), _volumen(volumen), _peso(peso) 
        {}
        
        // Método para obtener el nombre del objeto.
        string nombre() const {
            return _nombre;
        }

        // Método para obtener el volumen del objeto.
        double volumen() const {
            return _volumen;
        }

        // Método virtual para obtener el peso del objeto.
        virtual double peso() const {
            return _peso;
        }

        // Método virtual para mostrar información sobre el objeto.
        virtual string mostrar(const string& sep) const {
            // Utiliza un stringstream para construir la representación del objeto en forma de cadena.
            stringstream ss;
            ss << sep << _nombre << " [" << volumen() << " m3] [" << peso() << " kg]\n";
            return ss.str();
        }

        // Método virtual para obtener el tipo de objeto.
        virtual string tipo() const {
            return " de Carga Estandar";
        }
};