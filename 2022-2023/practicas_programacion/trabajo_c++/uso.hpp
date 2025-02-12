/*********************************************************************************************\
 * Programación 1. Trabajo
 * Autores: Irene Pascual y Rubén Agustín
 * Resumen: Fichero de interfaz «usos.hpp» de un módulo para trabajar con
 *          registros que representan usos del sistema Bizi Zaragoza.
\*********************************************************************************************/

#pragma once

#include <istream>
using namespace std;

struct UsoBizi {
    // Define los campos necesarios para representar los siguientes datos de un
    // uso del sistema Bizi: el identificador del usuario que utiliza la
    // bicicleta, el código de la estación de la que se retira la bicicleta y el
    // código de la estación en la que se devuelve.

    unsigned id;
    unsigned retiroEstacion;
    unsigned anclajeEstacion;
    string retiroDt;
    string anclajeDt;
    bool tipoUso;

};


/*
 * Pre:  El flujo «f» está asociado con un fichero de texto con el formato de usos del sistema
 *       Bizi establecido en el enunciado y en disposición de leer desde el principio de una
 *       línea distinta a la de la cabecera.
 * Post: Intenta leer la línea mencionada en la precondición y, si esta línea existe, almacena
 *       en los campos del parámetro «uso» el identificador del usuario al que corresponde el
 *       uso Bizi de la línea leída y los códigos de las estaciones de retirada y devolución de
 *       la bicicleta. Devuelve «true» si no se han terminado los datos del fichero en el
 *       intento de lectura descrito y y «false» en caso contrario.
 */
bool leerUso(istream& f, UsoBizi& uso);


/*
 * Pre:  La cadena de caracteres «nombreFicheroUsos» representa la ruta de acceso y el nombre
 *       de un fichero de texto con el formato de usos del sistema Bizi establecido en el
 *       enunciado.
 * Post: Si el fichero de nombre «nombreFicheroUsos» se puede abrir y leer correctamente,
 *       «traslados» es el número de usos entre estaciones distintas del sistema Bizi Zaragoza
 *       contenidos en el fichero de nombre «nombreFicheroUsos» y «usosCirculares» es el número
 *       de usos contenidos en dicho fichero que tienen como origen y destino la misma 
 *      estación.  En ese caso, devuelve «true» y en el caso contrario, «false».
 */
bool contarUsos(const string nombreFicheroUsos, unsigned& traslados, unsigned& usosCirculares);
