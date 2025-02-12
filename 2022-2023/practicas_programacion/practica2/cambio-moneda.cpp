/*******************************************************************************
 * Curso de Programación 1. Práctica 2
 * Autores: Miguel Ángel Latre y Javier Martínez
 * Modificado por: Irene Pascual Albericio
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;


int main() {
    /*Declaro las variables*/
    double dinero;
    int cent;
    int peseta;

    /*Pedimos el dato*/
    cout << "Escriba una cantidad no negativa de dinero en euros: ";
    cin >> dinero;

    /*Estos son los calculos para poder calcular los céntimos y las pesetas,*/
    /*gracias a un redondeo a la alza, y otro redondeo a la baja con el int*/
    cent=int(round((dinero-int(dinero))*100));
    peseta=round(dinero*166.386);

    cout << " Tiene " << int(dinero) << " euros, " << cent << " centimos, que equivalen a " << peseta << " pesetas.";

}
