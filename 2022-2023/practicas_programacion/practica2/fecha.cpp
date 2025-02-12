/*******************************************************************************
 * Curso de Programación 1. Práctica 2
 * Autores: Miguel Ángel Latre y Javier Martínez
 * Modificado por: Irene Pascual Albericio
 ******************************************************************************/

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    /*Declaro las variables*/
    unsigned numero1;
    unsigned numero2;
    unsigned year;
    unsigned mes;
    unsigned dia;

    /*Pedimos el dato*/
    cout << "Escriba una fecha con formato de 8 cifras [aaaammdd]: ";
    cin >> numero1;

    /*Mediante cocientes y restos calculamos por separado el año, el mes y el día*/
    numero2=numero1/100;
    dia=numero1%100;
    year=numero2/100;
    mes=numero2%100;

    /*Enviamos la solución escrita en el formato que se nos pide, rellenando con 0 si fuese necesario*/
    cout << "La fecha escrita es " << setfill('0') << setw(2) << dia << "/" << setw(2) << mes << "/" << setw(4) << year;

}
