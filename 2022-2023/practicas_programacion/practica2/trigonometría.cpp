/*******************************************************************************
 * Curso de Programación 1. Práctica 2
 * Autores: Miguel Ángel Latre y Javier Martínez
 * Modificado por: Irene Pascual Albericio
 ******************************************************************************/

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    /*Declaro las variables*/
    double grados;
    double grados1;
    double grados2;
    double grados3;
    double minutos;
    double segundos;
    double radianes;

    /*Pedimos el dato*/
    cout << " Escriba el valor de un angulo (grados, minutos y segundos): ";
    cin >> grados1 >> minutos >> segundos;

    /*Pasamos los minutos a grados y los segundos a grados*/
    /*A continuación sumamos todo para tener el dato completamente en grados*/
    grados2=minutos/60;
    grados3=segundos/3600;
    grados=grados1+grados2+grados3;

    /*Convertimos el dato que estaba en grados, en radianes*/
    radianes=(grados * 2 * M_PI)/360;

    /*Calculamos el seno, coseno y tangente*/
    cout << "Valor del angulo en radianes: " << radianes << endl;
    cout << " sin " << radianes << " = " << sin(radianes) << endl;
    cout << " cos " << radianes << " = " << cos(radianes) << endl;
    cout << " tg " << radianes << " = " << tan(radianes) << endl; 

}