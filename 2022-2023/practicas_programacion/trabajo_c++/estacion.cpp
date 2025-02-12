#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "estacion.hpp"
#include "uso.hpp"
#include "nombres-ficheros.hpp"

using namespace std;
/*
    Pre: Recibe un vector de tipo Estacion sin inicializar.

    Post: Inicializa el vector de estaciones de tal forma que el nombre y 
          usos que dan vacio y a 0 respectivamente, y la id se rellena de
          1 a NUM_ ESRACIONES ordenadamente.
*/
void inicializarEstaciones(Estacion estaciones[])
{
    for(unsigned i=0 ; i < NUM_ESTACIONES; i++)
    {
        estaciones[i].id = i+1;
        estaciones[i].nombre = " ";
        estaciones[i].numUsos = 0;
    }
}

bool leerEstaciones(const string nombreFicheroEstaciones, Estacion estaciones[]){
    string linea;
    unsigned identificador=0;
    inicializarEstaciones(estaciones);
    
    ifstream f(nombreFicheroEstaciones);
    if(f.is_open()){
        getline(f,linea);
        while(getline(f,linea,'=')){
            f >> identificador;
            
            //Aqui indico, por ejemplo, si leo el identificador 31, me lleva al vector[30] y me suma 1.
            //Si volviese a aparecer el identificador 31, sumaria otro al vector[30] y seria igual a 2 (porque
            //se han hecho dos usos de la misma estacion)
            getline(f,linea,'"');
            getline(f,linea,'"');
            getline(f,linea,'"');
            estaciones[identificador-1].nombre = linea;
            getline(f,linea);
           
        
        }

        
        f.close();
        return true;

    }
    else{
        return false;
    }

}

bool contarUsosEstaciones(const string nombreFicheroUsos, Estacion v_estaciones[]){
    string linea;
    unsigned identificador=0;
    
    ifstream g(nombreFicheroUsos);
    if(g.is_open()){
        getline(g ,linea);
        while(getline(g,linea,';')){
            
            getline(g,linea,';');
            getline(g,linea,';');
            identificador = stoi(linea);
            if(identificador <= NUM_ESTACIONES)
                v_estaciones[identificador-1].numUsos++;
            getline(g,linea,';');
            getline(g,linea);
            identificador = stoi(linea);
            if(identificador <= NUM_ESTACIONES)
                v_estaciones[identificador-1].numUsos++;

            
        }

        
        g.close();
        return true;

    }
    else{
        return false;
    }
}
/*
    Pre: Recibe un vector de estaciones previamente rellanado, el tamaño del vector a recorrer
         y dos parametros de entrada que guardaran la posición del valor mayor y menor, que deberán
         estar previamente inicializados a 0
    
    Post: Devuelve en los parametros posMayor y posMenor la posición en el vector dado de los valores
          mayor y menor correspondientemente.
*/
void busquedaMayorMenor(const Estacion estaciones[],const unsigned size, unsigned &posMayor, unsigned &posMenor)
{

    unsigned mayor = 0, menor= 99999999;
    for(unsigned i = size; i < NUM_ESTACIONES-size; i++)
    {
        if(mayor < estaciones[i].numUsos)
        {
            posMayor = i;
            mayor = estaciones[i].numUsos;
        }
        else if(menor > estaciones[i].numUsos)
        {
            posMenor = i;
            menor = estaciones[i].numUsos;
        }
    }

    
}
/*
    Pre: Recibe como parametro un vector previamente rellenado de tipo estación y las dos posiciones
         de intercambio que deben estar dentro del rango del tamaño del vector.
    
    Post: Intercambia el valor de la posción original, por el valor de la posición destino,
          sin que ninguno de los dos sea eliminado en el intercambio.
*/
void cambiarPosicion(Estacion estaciones[], const unsigned posicionOriginal,const unsigned posicioDestino)
{
    Estacion auxiliar;
    auxiliar = estaciones[posicioDestino];
    estaciones[posicioDestino] = estaciones[posicionOriginal];
    estaciones[posicionOriginal] = auxiliar;

}
/*
    Pre: Recibe un vector de tipo estación previamente rellenado

    Post: Recorre la mitad de la posicones del vector para reordenarlo, mandando
          el valor mas grande a la posición más alta de dicho vector y el valor
          más pequeño a la posción más alta posible de dicho vector.
*/
void ordenarPorUso(Estacion estaciones[])
{
    unsigned  posicionMayor = 0,posicionMenor = 0;

    for(unsigned j= 0; j < NUM_ESTACIONES/2;j++)
    {
        busquedaMayorMenor(estaciones, j, posicionMayor, posicionMenor);
        cambiarPosicion(estaciones,posicionMayor,j);
        cambiarPosicion(estaciones,posicionMenor,NUM_ESTACIONES-j-1);
        
    }
}
/*
    Pre: Recibe el flujo de salida del informe previamente abierto y un vector
         de estaciones relleno correctamente.

    Post: Muestra por pantalla la información del vector estaciones para cumplir con
          los requisitos del informe.
*/
void mostrarInformeEstaciones(ostream &informe, const Estacion estaciones[])
{
    informe << "Puesto "<< setw(8) << "Usos " << setw(4) << "Id  "
                        << "Nombre" << endl;
    informe << setfill('-')<< setw(7)<< " " << setw(8) << " "<< setw(4) 
            <<" " << setw(50)<< ""<<endl ;
    for(unsigned i = 0; i < NUM_ESTACIONES;i++)
    {
        informe <<setfill(' ')<< setw(6) << i+1 << " " << setw(7)<< estaciones[i].numUsos
                << " " << setw(3) << estaciones[i].id << " " << estaciones[i].nombre << endl;
    }
}

bool escribirInformeEstaciones(const string nombreFichero, const Estacion estaciones[])
{
    ofstream informe(nombreFichero);
        
    if(informe.is_open())
    {
        
        
        mostrarInformeEstaciones(informe,estaciones);
        
        informe.close();
        return 1;
        
    }
    else
    {
        
        return 0;
    }
    

}

bool contarViajesOrigenDestino(const string nombreFicheroUsos, 
                               unsigned viajes[][NUM_ESTACIONES])
{
    string cabezera;
    for(unsigned i = 0; i < NUM_ESTACIONES;i++)
    {
        for(unsigned j = 0; j< NUM_ESTACIONES; j++)
        {
            viajes[i][j]=0;
        }
    }
    ifstream usos(nombreFicheroUsos);
    UsoBizi bizi;
    if(usos.is_open())
    {
        getline(usos,cabezera);
        while(leerUso(usos,bizi))
        {
            if(bizi.retiroEstacion <= NUM_ESTACIONES && bizi.anclajeEstacion <= NUM_ESTACIONES)
            {
                viajes[bizi.retiroEstacion-1][bizi.anclajeEstacion-1]++;
            }
        }

        usos.close();
        return 1;
    }
    else{
        return 0;
    }
}
/*
    Pre: Recibe una fila del vector de viajes correctamente relleno.

    Post: Devuelve la posición del valor más alto en el vector de viajes.
*/
unsigned encontrarMasFrecuente(const unsigned viajes[])
{
    unsigned mayor = 0,indiceFrecuente;
    for(unsigned i = 0; i < NUM_ESTACIONES; i++)
    {
        if(mayor < viajes[i])
        {
            mayor = viajes[i];
            indiceFrecuente = i;
        }
    }
    return indiceFrecuente;
}

void calcularDestinosMasFrecuentes(const unsigned viajes[][NUM_ESTACIONES],
                                   unsigned destinosMasFrecuentes[])
{
    for(unsigned i = 0; i < NUM_ESTACIONES;i++)
    {
        destinosMasFrecuentes[i] = encontrarMasFrecuente(viajes[i]);
    }
    
}
/*
    Pre: Recibe un flujo de salida abierto, el caracter de relleno de la cabezera
         y cuanto espaciado se quiere en la cabezera
    Post: Escribe la cabezera de detinos.
*/
void cabezeraDestinos(ostream &f,const char relleno,const unsigned espaciado)
{
    f << "Viajes  " <<"Origen --> "<<"Destino" << endl 
      << setfill(relleno) <<setw(espaciado) <<""<<setfill(' ')<<endl;
}
/*
    Pre: Recibe un flujo de salida abierto, el vector viaje spreviamente relleno,
         el vector de destinos frecuente previamente relleno y el vector de estaciones
         previamente relleno.
         
    Post: Escribe el cuerpo de destino tal y como se pide en las especificaciones.
*/
void cuerpoDestinos(ostream &f,const unsigned viajes[][NUM_ESTACIONES], 
                    const unsigned destinosMasFrecuentes[],Estacion estaciones[])
{
    for(unsigned i = 0; i < NUM_ESTACIONES;i++)
    {
        f << setw(4) << viajes[i][destinosMasFrecuentes[i]] << setw(6) <<i+1
          << "-" << estaciones[i].nombre <<" -->  " << destinosMasFrecuentes[i]+1
          << "-" << estaciones[destinosMasFrecuentes[i]].nombre << endl;
    }
}

void escribirInformeDestinos(ostream& f, const unsigned viajes[][NUM_ESTACIONES], 
                             const unsigned destinosMasFrecuentes[])
{
    Estacion estaciones[NUM_ESTACIONES];
    leerEstaciones(FICHERO_ESTACIONES,estaciones);
    cabezeraDestinos(f,'-',120);
    cuerpoDestinos(f,viajes,destinosMasFrecuentes,estaciones);
    
}

