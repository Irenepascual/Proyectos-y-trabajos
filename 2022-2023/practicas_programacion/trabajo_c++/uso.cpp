#include "uso.hpp"
#include <fstream>
#include <string>

using namespace std;

const char DELIMITADOR  = ';';

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
bool leerUso(istream& f, UsoBizi& uso)
{
    string contenido;
    if(getline(f,contenido ,DELIMITADOR))
    {
        uso.id= stoi(contenido);
        getline(f,uso.retiroDt,DELIMITADOR);
        getline(f,contenido,DELIMITADOR);
        uso.retiroEstacion = stoi(contenido);
        getline(f,uso.anclajeDt,DELIMITADOR);
        getline(f,contenido);
        uso.anclajeEstacion = stoi(contenido);
        if(uso.retiroEstacion == uso.anclajeEstacion)
        {
            uso.tipoUso = true;
        }
        else
        {
            uso.tipoUso = false;
        }
        return true;
    }
    else{
        return false;
    }
    
}
/*
    Pre: Recibe un flujo de entrada previamente abierto, una variable de tipo
         Usobizi que no tiene que estar inicializada, además de dos variables
         traslados y circulares que deben valer 0.
    
    Post: Lee linea a linea el fichero asociado al flujo y incrementa las variables
          traslados y usosCirculares según se sean los traslados en el fichero que se
          está leyendo.
*/
void contarUsos(istream &f, UsoBizi &bizi, unsigned& traslados,
                unsigned& usosCirculares)
{
    string contenido;
    getline(f,contenido);
    while(leerUso(f,bizi))
    {
        if(bizi.tipoUso)
        {
            usosCirculares++;
        }
        else{
            traslados++;
        }
    }
}
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
bool contarUsos(const string nombreFicheroUsos, unsigned& traslados, unsigned& usosCirculares)
{
    
    UsoBizi bizi;
    ifstream f(nombreFicheroUsos);
    if( f.is_open())
    {
        
        contarUsos(f,bizi,traslados,usosCirculares);

        f.close();
        return true;
    }
    else{
        return false;
    }
}