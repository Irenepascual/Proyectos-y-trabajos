#pragma once
#include <iostream>
using namespace std;

// Clase base para excepciones relacionadas con el árbol de ficheros
class arbol_ficheros_error {
    protected:
        string _mensaje; // Mensaje de error asociado a la excepción
    public:
        //obtener el mensaje de error
        virtual string what() const {
            return _mensaje;
        }
};

// Excepción lanzada cuando no se encuentra un nodo en una ruta específica
class noExisteNodo : public arbol_ficheros_error {
    public:
        noExisteNodo() {
            _mensaje = "no existe un nodo con ese nombre en ese path";
        }
};

// Excepción lanzada cuando ya existe un nodo con el mismo nombre en el directorio actual
class yaExisteNodo : public arbol_ficheros_error {
    public:
        yaExisteNodo() {
            _mensaje = "ya existe un nodo con ese nombre en el directorio actual";
        }
};

// Excepción lanzada cuando ya existe un nodo con el mismo nombre que no es un fichero en el directorio actual
class yaExisteNodoNoFichero : public arbol_ficheros_error {
    public:
        yaExisteNodoNoFichero() {
            _mensaje = "ya existe un nodo con ese nombre en el directorio actual que no es un fichero";
        }
};

// Excepción lanzada cuando se intenta acceder a un directorio padre que no existe
class noHayDirAnterior : public arbol_ficheros_error {
    public:
        noHayDirAnterior() {
            _mensaje = "no existe el directorio padre";
        }
};

// Excepción lanzada cuando el nodo no es un directorio en el path especificado
class noEsDirectorio : public arbol_ficheros_error {
    public:
        noEsDirectorio() {
            _mensaje = "el nodo no es un directorio en ese path";
        }
};

// Excepción lanzada cuando el nodo apuntado por un enlace no es un directorio
class apuntadoNoEsEnlace : public arbol_ficheros_error {
    public:
        apuntadoNoEsEnlace() {
            _mensaje = "el nodo apuntado por el enlace no es un directorio";
        }
};

// Excepción lanzada cuando el directorio apuntado por un enlace no existe
class enlaceInaccesible : public arbol_ficheros_error {
    public:
        enlaceInaccesible() {
            _mensaje = "el directorio apuntado por el enlace no existe";
        }
};
