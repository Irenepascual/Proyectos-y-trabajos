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
    unsigned numero;
    numero=0;

    /*Pedimos el dato*/
    cout << "Escriba un entero entre 1 y 10: ";
    cin >> numero;

    /*Declaramos lo que debe aparecer en pantalla dependiendo*/
    /*del dato, que es un valor entre el 1 y el 10*/
    if (numero==1) {
        cout << numero << " = I";
    }
    else {
        if (numero==2) {
        cout << numero << " = II";
        }
        else {
            if (numero==3) {
            cout << numero << " = III";
            }
            else {
                if (numero==4) {
                cout << numero << " = IV";
                }
                else {
                   if (numero==5) {
                   cout << numero << " = V";
                   }
                   else {
                        if (numero==6) {
                        cout << numero << " = VI";
                        }
                        else {
                            if (numero==7) {
                            cout << numero << " = VII";
                            }
                            else {
                                if (numero==8) {
                                cout << numero << " = VIII";
                                }
                                else {
                                    if (numero==9) {
                                    cout << numero << " = IX";
                                    }
                                    else {
                                        cout << numero << " = X";
                                    }
                                }
                            }
                        }
                   }
                }
            }
        }
    }

}
        

