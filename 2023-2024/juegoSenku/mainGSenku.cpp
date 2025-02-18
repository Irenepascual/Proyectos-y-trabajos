// Autores: Irene  Pascual Albericio  ; Sergio Garcés Maluenda      

#include <iostream>
#include <unistd.h>
#include "GSenku.hpp"

using namespace std;


int main(int argc, char* argv[]) {

    tpTablero tablero;
    tpMovimientosValidos movimientos;
    tpMovimientosValidos movValidos;
    tpListaMovimientos solucionParcial;
    tpListaMovimientos solucion;

    string ficheroMovimientos;
    string ficheroTablero;
    int retardo;
    string ficheroSalida;

    if (argc > 4) {
        ficheroTablero = argv[1];
        ficheroMovimientos = argv[2];
        retardo = stoi(argv[3]);
        ficheroSalida = argv[4];
    } else if (argc == 4) {
        ficheroTablero = argv[1];
        ficheroMovimientos = argv[2];
        retardo = stoi(argv[3]);
        ficheroSalida = "resultado.txt"; // Valor por defecto para ficheroSalida
    }

    // Inicializar tablero y movimientos
    if (inicializarTablero(ficheroTablero, tablero) && inicializarMovimientosValidos(ficheroMovimientos, movValidos)){

        // Buscar solución
        if (buscaSolucion(tablero, movValidos, solucionParcial, retardo) == 1) {
        solucion = solucionParcial;
        escribeListaMovimientos(ficheroSalida, solucion);
        cout << "Solución final: " << endl;
        mostrarTablero(tablero);
        cout << "Se ha encontrado una solucion." << endl;
        return 0;
        } else {
        escribeListaMovimientos(ficheroSalida, solucion);
        cout << "No se ha encontrado una solucion." << endl;
        return -1;
        }
       
    }
    else {
        cout << "No se ha podido leer el fichero de tablero o el de movimientos." << endl;
        return -1;
    }
}

