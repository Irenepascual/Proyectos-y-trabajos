/*******************************************************************************
 * Curso de Programación 1. Tema 1
 * Autores: Miguel Ángel Latre
 * Modificado por: Irene Pascual Albericio
 * Última revisión: 18 de septiembre de 2021
 * Resumen: Tarea 3 y 4
 ******************************************************************************/
#include <iostream>
using namespace std;

/*
 * Programa que solicita un número entero al usuario y lo escribe en pantalla.
 */
int main() {
    cout << "Escriba un entero: ";
    
    int numero1;
    cin >> numero1;

    cout << "Escriba otro entero: ";
    
    int numero2;
    cin >> numero2;
    
    cout << "El entero escrito es el " << numero1+numero2 << endl;


    cout << "Escriba dos enteros separados por un espacio: "; 
    
    int numero3,numero4; 
    cin >> numero3 >> numero4;

    cout << "Su suma es: " << numero3+numero4 << endl;
}
