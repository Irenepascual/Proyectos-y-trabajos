//------------------------------------------------------------------------------
// File:   practica_2.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar
// Date:   septiembre 2023
// Coms:   Ejercicio 1 de la Practica 1.
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

void nums(int id, int retardo, int veces) {

    for (int i=1; i<=veces; i++){
        // cout << "Soy " << nuumero aleatorio del 0 al 9 << endl;
        cout << "Soy " + to_string(id) + "\n";
        //el thread que me ejecuta se bloquea durante "retardo" milisegundos, entre 100 y 300 ms.
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

int main(int argc, char* argv[]) { 
    const int N = 10;
    thread P[N]; //de momento, ningún thread se pone en marcha
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));//Para calcular números aleatorios de tipo int.

    //El primer rand() calcula numeros del 0 al 9
    //El segundo rand() calcula numeros del 100 al 300
    //El tercer rand() calcula numeros del 5 al 15
    //A su vez, se ejecuta este proceso durante 10 veces, gracias al bucle.
    for (int i = 0; i < N; i++)
    {
        P[i] = thread(&nums, (std::rand()%10), (std::rand()%201+100), (std::rand()%11 +5));
    }

    for (int i=0; i<N; i++) { 
        P[i].join(); //me bloqueo hasta que "P[i]" termine
    }

    cout << "Fin\n";
    return 0;
}
