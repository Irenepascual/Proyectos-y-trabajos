#pragma once
#include <iostream>
#include <sstream>
#include <memory>
using namespace std;

class Nodo{
    protected:
        string _nombre;
    public:
        Nodo(const string& nombre){
            _nombre=nombre;
        }

        ~Nodo() {}

        // Devuelve el tamaño del nodo
        virtual int get_tam() const=0;
        
        // Devuelve el nombre del nodo
        string get_nombre() const{
            return _nombre;
        }
};