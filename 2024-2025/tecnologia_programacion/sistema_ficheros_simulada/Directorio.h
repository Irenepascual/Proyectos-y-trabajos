#pragma once
#include "Nodo.h"
#include <vector>
#include <iomanip>

class Directorio: public Nodo {
    protected:
        int _tam;
    public:
        vector<shared_ptr<Nodo>> _elems;

        Directorio(const string& nombre, const int& tam)
            : Nodo(nombre), _tam(tam)
        {}

        ~Directorio() {}

        // Devuelve el tamaño del directorio 
        //(suma de los tamaños de los nodos contenidos)
        int get_tam() const override {
            int tot=0;
            for (auto i : _elems) {
                tot = tot + i->get_tam();
            }
            return tot;
        }

        // Inserta un elemento en el directorio
        void insert_elem(shared_ptr<Nodo> elem) {
            _elems.push_back(elem);
        }

        // Obtiene un nodo por el nombre indicado
        shared_ptr<Nodo> obtener_nodo(string name) {
            for (auto i : _elems) {
                if (name == i->get_nombre()) {
                    return i;
                }
            }
            return nullptr;
        }

        // Borra un nodo
        void borrar_nodo(shared_ptr<Nodo> nodo_a_borrar) {
            bool encontrado = false;
            int i = 0;
            while (i<(int)_elems.size() && !encontrado) {
                if (_elems[i] == nodo_a_borrar) {
                    encontrado = true;
                    _elems.erase(_elems.begin()+i);
                }
                i++;
            }
        }

        // Borra todos los nodos del directorio
        void borrar_todos_nodos() {
            _elems.erase(_elems.begin(),_elems.end());
        }

        // Muestra los nombres de los nodos en el directorio
        void mostrar_nombre(stringstream& ss) const {
            for (auto i : _elems) {
                ss << "   "<< i->get_nombre() << endl;
            }
        }

        // Muestra los nombres y tamaños de los nodos en el directorio
        void mostrar_nombre_tam(stringstream& ss) const {
            for (auto i : _elems) {
                ss << "   " << i->get_nombre();
                size_t espacio = 40 - (i->get_nombre()).length();
                for (size_t j=0; j < espacio; j++){
                    ss << ".";
                }
                ss << i->get_tam() << endl;
            }
        }
};