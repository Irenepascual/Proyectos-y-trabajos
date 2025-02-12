/*********************************************************************************************\
 * Programación 1. Trabajo 
 * Autores: Irene Pascual y Rubén Agustín
 * Resumen: Fichero de interfaz «nombres-ficheros.hpp» de un módulo que contiene constantes con
 *          los nombres o partes de nombres de los ficheros que se van a utilizar y un
 *          procedimiento denominado «construirNombreFicheroUsos» que facilita la construcción
 *          del nombre de un fichero de usos a partir de la opción sobre el fichero de usos que
 *          haya escrito el usuario. 
\*********************************************************************************************/

#pragma once

#include <string>
using namespace std;

const string RUTA_DATOS = "datos/";
const string PREFIJO_USOS = "usos-";
const string EXTENSION = ".csv";
const string FICHERO_ESTACIONES = RUTA_DATOS + "estaciones.csv";
const string FICHERO_USUARIOS = RUTA_DATOS + "usuarios.csv";
const string FICHERO_AYUDA = "res/ayuda.txt";
const string FICHERO_OPCIONES = "res/opciones.txt";


/*
 * Pre:  ---
 * Post: Devuelve una ruta de acceso relativa a un fichero de usos partiendo del valor del 
 *       parámetro «opcion», concatenando la ruta relativa de acceso al directorio de datos con
 *       el prefijo común a los ficheros de usos, la opción y la extensión de los fichero de
 *       usos.
 *       Por ejemplo, si «opcion» tiene el valor "16", devuelve la ruta "datos/usos-16.csv".
 *       Si «opcion» tiene el valor "t2", devuelve la ruta "datos/usos-t2.csv".
 */
string construirNombreFicheroUsos(const string opcion);
