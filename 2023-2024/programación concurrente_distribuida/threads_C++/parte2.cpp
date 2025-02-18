//------------------------------------------------------------------------------
// File:   practica_3.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar
// Date:   septiembre 2023
// Coms:   Ejercicio 2 de la Practica 1.
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>
#include <cmath>
using namespace std;

//Esta función calcula la media del vector, lo recorre y va haciendo
//una suma parcial hasta que llega hasta el último componente.
//Se pasa por referencia la variable fin, que avisará cuando esté
//realizado el cálculo de la media para poder usarlo en otra función
//denominada "desvi".
void medi(double vec[], int tamano, double &media, bool &fin) {
    fin=false;
    double aux=0;
    for (int i = 0; i < tamano; i++)
    {
        aux=vec[i]+aux;
    }
        media = (aux/tamano);
        fin=true;
    }

//Esta función calcula el maximo y el minimo del vector, al igual que 
//el anterior, los va recorriendo y comprobando si el componente que 
//están leyendo es mayor o menor que los anteriores, para convertirse
//de ese modo en maximos o minimos absolutos.
void maximini(double vec[], int tamano, double &maximo, double &minimo){
    maximo=vec[0];
    minimo=vec[0];
    for (int i = 1; i < tamano; i++)
    {
        if (vec[i]>maximo)
        {
            maximo=vec[i];
        }
        if (vec[i]<minimo)
        {
            minimo=vec[i];
        }
    }
}

//Esta función, empieza a realizar las operaciones de la desviación
//una vez la media ya ha sido calculada. Va recorriendo el vector y se
//realiza la suma al cuadrado de la diferencia del componente con 
//respecto a la media (y esto lo hace con todos los componentes del vector).
//Posteriormente, realiza la raiz cuadrada del resultado anterior, que es la
//varianza, para obtener la desviación.
void desvi(double v[], const int tamano, double& desv, double media, bool fin){
    while(!fin)
    {
    }
    for (int i = 0; i < tamano; i++) {
        desv += pow(v[i]-media, 2);
    }
    desv = sqrt(desv/tamano-1);
}

int main(int argc, char* argv[]) {
    std::srand(static_cast<double>(std::time(nullptr))); //Código para obtener números aleatorios de tipo double
    //Declaramos las variables globales
    const int tamano = 100;
    bool fin = false;
    double media = 0.0;
    double max = 0.0;
    double min = 0.0;
    double desviacion = 0.0;
    thread P[3];
    double vec[tamano];

    //Creamos el vector de tamaño 100, con números aleatorios
    for (int i = 0; i < tamano; i++)
    {
       vec[i]=std::rand();
    }
 
    //Ejecutamos los hilos, que llaman a las funciones, para que se produzcan de forma concurrente.
    P[0] = thread(&medi,vec,tamano,ref(media),ref(fin));
    P[1] = thread(&maximini,vec,tamano,ref(max),ref(min));
    P[2] = thread(&desvi,vec,tamano,ref(desviacion),media,fin);

    //El programa no continúa hasta que los hilos se han terminado de ejecutar, y no pasa a escribir los datos de 
    //a continuación.
    for (int i=0; i<3; i++) {
        P[i].join();
    }

    //Datos que escribe por pantalla:
    cout << "#datos " << tamano << endl;
    cout << "media: " << media << endl;
    cout << "máx: " << max << endl;
    cout << "mín: " << min << endl;
    cout << "sigma: " << desviacion << endl;
    cout << "Fin\n";
    return 0;
}
