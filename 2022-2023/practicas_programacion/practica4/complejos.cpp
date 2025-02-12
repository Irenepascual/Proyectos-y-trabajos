/******************************************************************************\
 * Programación 1. Práctica 4
 * Autor: Irene Pascual Albericio
 * Ultima revisión: ¡¡¡¡¡¡¡¡¡¡¡!!!!!!!!!
 * Resumen: Fichero de implementación del módulo principal del programa que 
 *          trabaja con números complejos.
 * Nota: El código de este programa no está repartido en varios módulos, por lo
 *       que se puede utilizar la extensión Code Runner de VSC para ejecutarlo.
 *       No obstante, se ha configurado el fichero «Makefile» para poder 
 *       compilarlo y se han definido tareas de VSC para compilarlo, ejecutarlo 
 *       y depurarlo.
 *       Para compilarlo, se puede ejecutar el comando
 *           make complejos
 *       o, en Windows,
 *           mingw32-make complejos
 *       o ejecutar la tarea "Compilar «complejos»" de VSC
 * 
 *       Para ejecutarlo, una vez compilado, se puede ejecutar el comando
 *           bin/complejos
 *       o, en Windows,
 *           bin\complejos.exe
 *       o ejecutar la tarea "Ejecutar «complejos»" de VSC
 *       o compilarlo y ejecutarlo con la extensión Code Runner (Ctrl+Alt+N).
\******************************************************************************/
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;

struct complejo{
    double partereal;
    double parteimaginaria;
};

void crearComplejo(int x, int y, unsigned n, complejo numeros[]){
    for (unsigned i=0; i<n; i++){
        numeros[i].partereal= ((double)rand()/RAND_MAX)*(y-x)+x;
        numeros[i].parteimaginaria= ((double)rand()/RAND_MAX)*(y-x)+x;
    }
}

void mostrarComplejo(complejo p){
    if (p.parteimaginaria>0){
        cout << p.partereal << "+" << p.parteimaginaria << "i";
    }
    else{
        cout << p.partereal << "" << p.parteimaginaria << "i";
    }
}

void mostrarNumeros(complejo numeros[],unsigned n){
    for(unsigned i=0 ; i<n; i++){
        mostrarComplejo(numeros[i]);
        if(i != n-1){
            cout << ", ";
        }
    }
}

complejo sumar(complejo numeros[],unsigned n){
    complejo suma = {0,0};
    for(unsigned i = 0 ; i<n;i++){
        suma.partereal+=numeros[i].partereal;
        suma.parteimaginaria+=numeros[i].parteimaginaria;
    }
    return suma;
}

double modulo(complejo p){
    return sqrt(pow(p.partereal,2)+pow(p.parteimaginaria,2));
}
 
int mayorComplejo(complejo numeros[], unsigned n){
    int mayorModulo= modulo(numeros[0]);
    for (unsigned i=0; i<n; i++){
        if(modulo(numeros[i])>mayorModulo){
            mayorModulo=modulo(numeros[i]);
        }
    }
    return mayorModulo;
}

int menorComplejo(complejo numeros[], unsigned n){
    int menorModulo= modulo(numeros[0]);
    for (unsigned i=0; i<n; i++){
        if(modulo(numeros[i])>menorModulo){
            menorModulo=modulo(numeros[i]);
        }
    }
    return menorModulo;
}

void pedirorden(unsigned& x, unsigned& y){
    cout<<"Introduce los limites del intervalo (x < y): ";
    cin >> x >> y;
    while (x>y || x==y){
        cout<<"Introduce los limites del intervalo (x < y): ";
        cin >> x >> y;
    }
}

int main() {
    unsigned x, y;
    int numeroComplejos=5;
    complejo num[numeroComplejos];

    cout<<"Introduce los limites del intervalo (x < y): ";
    cin >> x >> y;

    while (x>y || x==y){
    cout<<"Introduce los limites del intervalo (x < y): ";
    cin >> x >> y;
    }

    cout << fixed << setprecision(3);

    if(x < y){
        crearComplejo(x,y,numeroComplejos,num);

        cout << "Numeros complejos generados :" << endl;
        mostrarNumeros(num,numeroComplejos);
        cout << endl;

        cout << "Numeros complejos de mayor modulo:" << endl;
        mostrarComplejo(num[mayorComplejo(num,numeroComplejos)]);
        cout << endl;

        cout << "Numeros complejos de menor modulo:" << endl;
        mostrarComplejo(num[menorComplejo(num,numeroComplejos)]);
        cout << endl;
        
        cout << "Suma:" << endl;
        sumar(num,numeroComplejos);
        mostrarComplejo(sumar(num,numeroComplejos));
    }
    return 0;
}
