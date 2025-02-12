#include "usos-usuario.hpp"
#include "uso.hpp"
#include <fstream>

unsigned numUsosTotales(const UsosUsuario usuario)
{
    return usuario.circulares+usuario.traslados;
}
/*
    Pre:Recibe un vector tipo usos usuario previamente relleno, el número de usuarios que debe
        coincidir con el número de usuarios totales y el usuario que se desea buscar.

    Post:Devuelve la posición del vector donde se encuentra el usuario buscado, encaso de error
         devuelve -1.
*/
int buscarUsuario(const UsosUsuario usuario[],const unsigned numUsuarios,const unsigned usuarioBuscado)
{
    for(unsigned i = 0; i < numUsuarios;i++)
    {
        if(usuarioBuscado == usuario[i].id)
        {
            return i;
        }
    }
    return -1;
}
bool obtenerUsosPorUsuario(const string nombreFicheroUsos,
                           UsosUsuario usuarios[], unsigned& numUsuarios)
{
    string vacio;
    ifstream fichero(nombreFicheroUsos);
    UsoBizi uso;
    int indiceUsuario = 0;
    numUsuarios = 0;
    if(fichero.is_open())
    {
        getline(fichero,vacio);
        while(leerUso(fichero,uso))
        {
            indiceUsuario = buscarUsuario(usuarios,numUsuarios,uso.id);
            if(indiceUsuario < 0)
            {
                usuarios[numUsuarios].id = uso.id;
                if(!uso.tipoUso)
                    usuarios[numUsuarios].traslados++;
                else
                    usuarios[numUsuarios].circulares++;
                numUsuarios++;
            }
            else{
                if(!uso.tipoUso)
                    usuarios[indiceUsuario].traslados++;
                else
                    usuarios[indiceUsuario].circulares++;
            }
            
        }


        fichero.close();
        return true;
    }
    else
    {
        return false;
    }
}
/*
    Pre: Recibe un vector de usos usuario previamente rellanado, el tamaño del vector a recorrer
         y un parametros de entrada que guardará la posición del valor mayor , que deberá
         estar previamente inicializado a 0
    
    Post: Devuelve en los parametros posMayor ,la posición en el vector dado del valor
          mayor.
*/
void busquedaMayor(UsosUsuario usuarios[],const unsigned size, unsigned &posMayor,const unsigned numOrdenar)
{

    unsigned mayor = 0,valorActual;
    for(unsigned i = size; i < numOrdenar; i++)
    {
        valorActual = numUsosTotales(usuarios[i]);
        if(mayor < valorActual)
        {
            posMayor = i;
            mayor = valorActual;
        }
    }

    
}
/*
    Pre: Recibe como parametro un vector previamente rellenado de tipo usos usuario y las dos posiciones
         de intercambio que deben estar dentro del rango del tamaño del vector.
    
    Post: Intercambia el valor de la posción original, por el valor de la posición destino,
          sin que ninguno de los dos sea eliminado en el intercambio.
*/
void cambiarPosicion(UsosUsuario usuarios[], const unsigned posicionOriginal,const unsigned posicioDestino)
{
    UsosUsuario auxiliar;
    auxiliar = usuarios[posicioDestino];
    usuarios[posicioDestino] = usuarios[posicionOriginal];
    usuarios[posicionOriginal] = auxiliar;

}
void ordenar(UsosUsuario usuarios[], const unsigned numUsuarios, 
             const unsigned numOrdenar)
{
    unsigned  posicionMayor = 0;

    for(unsigned j= 0; j < numOrdenar;j++)
    {
        busquedaMayor(usuarios, j, posicionMayor,numUsuarios);
        cambiarPosicion(usuarios,posicionMayor,j);
        
    }
}