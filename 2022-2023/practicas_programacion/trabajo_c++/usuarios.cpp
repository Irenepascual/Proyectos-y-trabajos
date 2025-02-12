#include "usuarios.hpp"
#include <fstream>
#include <iostream>
const char DELIMITADOR = ';';

void obtenerEstadisticas(ifstream &f,unsigned estadisticas[][NUM_GENEROS])
{
    string contenido, genero, edad;
    int iGenero = 0;
    getline(f,contenido);
    while(getline(f,contenido,DELIMITADOR))
    {
        getline(f,genero,DELIMITADOR);
        getline(f,edad);
        iGenero = indiceGenero(genero);
        if(iGenero >= 0)
            estadisticas[indiceRangoEdad(edad)][iGenero]++;
    }
}

/*
 * Pre:  «nombreFicheroUsuarios» es el nombre de un fichero que cumple con la sintaxis de la
 *       regla <fichero-usuarios> establecida en el enunciado. La matriz «estadisticas» tiene
 *       «NUM_EDADES» filas y «NUM_GENEROS» columnas.
 * Post: Asigna a las componentes de la fila indexada por 0 el número de usuarios masculinos
 *       (columna 0) y usuarias femeninas (columna 1) el número de usuarios de 25 años o menos
 *       según el contenido del fichero «nombreFicheroUsuarios». Hace lo análogo en la fila
 *       indexada por 1 con los usuarios de entre 26 y 35 años; en la fila indexada por 2 con
 *       los usuarios de entre 36 y 50 años; en la fila indexada por 3 con los usuarios de
 *       entre 51 y 65 años y en la fila indexada por 4 con los usuarios de 65 años o más.
 *       Si se puede leer del fichero «nombreFicheroUsuarios», devuelve «true» y, en caso
 *       contrario, devuelve «false».
 */
bool obtenerEstadisticas(const string nombreFicheroUsuarios,
                         unsigned estadisticas[][NUM_GENEROS])
{
    
    ifstream f(nombreFicheroUsuarios);
    if( f.is_open())
    {   
        obtenerEstadisticas(f,estadisticas);
        f.close();
        return true;
    }
    else{
        return false;
    }
}



/*
 * Pre:  El valor del parámetro «rangoEdad» es uno de los valores válidos según la regla
 *       <rango-edad> establecida en el enunciado ("<=25", "26-35", "36-50", "51-65" o ">65")
 * Post: Devuelve, dependiendo del valor del parámetro «rangoEdad»
 *       ("<=25", "26-35", "36-50", "51-65" o ">65"), respectivamente, 0, 1, 2, 3 o 4.
 */
unsigned indiceRangoEdad(const string rangoEdad)
{
    for(unsigned i = 0; i < NUM_EDADES; i++)
    {
        if( rangoEdad == RANGO_EDADES[i])
        {
            return i;
            break;
        }
    }
    
    return 0;
}

/*
 * Pre:  ---
 * Post: Si el valor del parámetro «genero» es uno de los valores válidos según la regla
 *       <género> establecida en el enunciado ("M" o "F"), devuelve, respectivamente, 0 o 1.
 *       En caso contrario, devuelve -1.
 */
int indiceGenero(const string genero)
{
    if(genero == "M")
    {
        return 0;
    }
    else if(genero == "F")
    {
        return 1;
    }
    else{
        return -1;
    }

}

bool buscarUsuario(istream &f,const unsigned idUsuario,
                   string &genero, string &rangoEdad)
{
    string contenido;
    unsigned id;
    getline(f,contenido);
    while(f >> id)
    {
        
        if( idUsuario == id)
        {
            getline(f,contenido,DELIMITADOR);
            getline(f,genero,DELIMITADOR);
            getline(f,rangoEdad);
            return true;
        }
        getline(f,contenido);
        
    }
    return false;
}

/*
 * Pre:  «nombreFicheroUsuarios» es el nombre de un fichero que cumple con la sintaxis de la
 *       regla <fichero-usuarios> establecida en el enunciado.
 * Post: Si en el fichero «nombreFicheroUsuarios» hay datos de un usuario con identificador
 *       igual al valor del parámetro «idUsuario», devuelve «true» y asigna a los parámetros
 *       «genero» y «rangoEdad» el valor correspondiente a ese usuario presente en el fichero:
 *       "M", "F" o "" en el caso de «genero» y "<=25", "26-35", "36-50", "51-65" o ">65" en el
 *       caso de «rangoEdad».
 */
bool buscarUsuario(const string nombreFicheroUsuarios, const unsigned idUsuario,
                   string &genero, string &rangoEdad)
{
    
    ifstream f(nombreFicheroUsuarios);
    if( f.is_open())
    { 
        
        bool encontrado = buscarUsuario(f,idUsuario,genero,rangoEdad);
        
        f.close();
        return encontrado;

    }
    else{
        return false;
    }


}