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
    unsigned dinero;
    unsigned billete50;
    unsigned billete20;
    unsigned billete10;

    /*Pedimos el dato*/
    cout << " Cantidad a retirar en euros [positiva y multiplo de 10]: ";
    cin >> dinero;

    billete50=0;
    billete20=0;
    billete10=0;
    
    /*Priorizamos el orden para dividir el dinero,*/
    /*primero en billetes de 50 si se pudiese, después*/
    /*en los de 20 y sino en los de 10.*/
    if (dinero>=50) {
        billete50=dinero/50;
        dinero=dinero%50;
    }
    if (dinero>=20) {
        billete20=dinero/20;
        dinero=dinero%20;
    }
    if (dinero>=10) {
        billete10=dinero/10;
    }

    /*Establecemos la variable ancho para mantener constante el espacio*/
    const int ANCHO = 9;
    cout << right << setw(ANCHO) << "Billetes" 
    << right << setw(ANCHO) << "Euros" 
    << endl;

    cout << right << setw(ANCHO) << "========" 
    << right << setw(ANCHO) << "========" 
    << endl;
    
    cout << fixed;
    cout << right << setw(ANCHO) << billete10 
    << right << setw(ANCHO) << "10" 
    << endl;

    cout << right << setw(ANCHO) << billete20 
    << right << setw(ANCHO) << "20" 
    << endl;

    cout << right << setw(ANCHO) << billete50 
    << right << setw(ANCHO) << "50" 
    << endl;
}