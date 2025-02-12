/*********************************************************************************************\
 * Programación 1. Trabajo
 * Autores: Irene Pascual y Rubén Agustín
 * Resumen: Fichero de interfaz «usos-usuario.hpp» de un módulo para trabajar con registros que
 *          permiten contabilizar el número de usos que cada usuario hace del sistema Bizi.
\*********************************************************************************************/

#pragma once 

#include <string>
using namespace std;

struct UsosUsuario {
    // Define los campos necesarios para representar los siguientes datos sobre las
    // utilizaciones del sistema Bizi de determinado usuario: su identificador de usuario,
    // el número de usos entre estaciones distintas realizadas por ese usuario y el número de
    // usos entre la misma estación.
    unsigned id = 0;
    unsigned traslados = 0;
    unsigned circulares = 0;
};


/*
 * Pre:  ---
 * Post: Devuelve el número total de usos del sistema Bizi correspondiente a «usuario».
 */
unsigned numUsosTotales(const UsosUsuario usuario);


/*
 * Pre:  «nombreFicheroUsos» contiene la ruta y nombre de un fichero de texto con información
 *       sobre usos del sistema Bizi Zaragoza y con el formato establecido en el enunciado.
 *       El vector «usuarios» tiene al menos tantas componentes como número de usuarios
 *       distintos aparecen  en el fichero de nombre «nombreFicheroUsos». El valor del
 *       parámetro «numUsuarios» no está definido.
 * Post: Si se puede leer del fichero «nombreFicheroUsos», el vector «usuarios» almacena, en
 *       sus primeras «numUsuarios» componentes, la información relativa a identificadores de
 *       usuario y número de usos (entre estaciones distintas y entre la misma estación)
 *       extraída del fichero «nombreFicheroUsos» de acuerdo con las consideraciones
 *       establecidas en el enunciado. No es necesario que los registros del vector estén
 *       ordenados por ningún criterio en concreto. Devuelve «true» si el fichero puede ser
 *       leído sin problemas y «false» en caso contrario.
 */
bool obtenerUsosPorUsuario(const string nombreFicheroUsos,
                           UsosUsuario usuarios[], unsigned& numUsuarios);

/*
 * Pre:  numUsuarios > 0; numOrdenar > 0
 * Post: El vector usuarios[0..numUsuarios-1] es una permutación de los datos iniciales del
 *       vector, de forma que los «numOrdenar» primeros son los usuarios de mayor número de
 *       usos y están ordenados por número de usos decreciente.
 */
void ordenar(UsosUsuario usuarios[], const unsigned numUsuarios, 
             const unsigned numOrdenar);
