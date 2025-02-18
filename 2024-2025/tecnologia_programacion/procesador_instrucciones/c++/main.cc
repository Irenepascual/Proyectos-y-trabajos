#include "programa1.h"
#include "programa2.h"
#include "programa3.h"
using namespace std;

int main () {
    // PRUEBA PROGRAMA 1 - SUMA DE DOS NÚMEROS
    cout << "Primer programa: SUMA DE DOS NUMEROS" << endl;
    programa1 p1;
    cout << "Programa:" << endl;
    p1.listar();
    cout << endl << "Ejecucion: " << endl;
    p1.ejecutar();
    cout << endl;

    // PRUEBA PROGRAMA 2 - CUENTA ATRÁS
    cout << "Segundo programa: CUENTA ATRAS" << endl;
    programa2 p2;
    cout << "Programa:" << endl;
    p2.listar();
    cout << endl << "Ejecucion: " << endl;
    p2.ejecutar();
    cout << endl;

    // PRUEBA PROGRAMA 3 - FACTORIAL
    cout << "Tercer Programa: FACTORIAL" << endl;
    programa3 p3;
    cout << "Programa:" << endl;
    p3.listar();
    cout << endl << "Ejecucion: " << endl;
    p3.ejecutar();

}