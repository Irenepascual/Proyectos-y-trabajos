#pragma once
#include "Nodo.h"

class Enlace: public Nodo {
    protected:
        int _tam;
        shared_ptr<Nodo> _puntero;
    public:
        Enlace(const string& nombre, shared_ptr<Nodo> puntero)
            : Nodo(nombre), _tam(puntero->get_tam()), _puntero(puntero)
        {}

        ~Enlace() {}

        // Devuelve el tamaño del enlace 
        //(tamaño del nodo al que apunta)
        int get_tam() const override {
            return _puntero->get_tam();
        }

        // Devuelve el nodo al que apunta el enlace
        shared_ptr<Nodo> get_link() const {
            return _puntero;
        }
};