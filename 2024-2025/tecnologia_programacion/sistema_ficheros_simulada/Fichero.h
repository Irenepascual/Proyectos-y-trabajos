#pragma once
#include "Nodo.h"

class Fichero : public Nodo {
    protected:
        int _tam;
    public:
        Fichero(const string& nombre, const int& tam)
            : Nodo(nombre), _tam(tam)
        {}

        ~Fichero() {}

        // Devuelve el tamaño del fichero
        int get_tam() const override {
            return _tam;
        }

        // Establece el tamaño del fichero
        void set_tam(int size) {
            _tam = size;
        }
};