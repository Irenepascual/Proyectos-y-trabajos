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
   unsigned tiempo;
   unsigned segundos;
   unsigned minutos;
   unsigned horas;

    /*Pedimos el dato*/
   cout << "Duracion en segundos: ";
   cin >> tiempo;

   cout << "Este tiempo equivale a " ;
   
   /*Calculamos el tiempo priorizando los segundos, luego los minutos,*/
   /*después las horas y finalmente los días*/
   if (tiempo>=60) {
        segundos=tiempo%60;
        tiempo=tiempo/60;
        if (tiempo>=60) {
            minutos=tiempo%60;
            tiempo=tiempo/60;
            if (tiempo>=24) {
                horas=tiempo%24;
                tiempo=tiempo/24;
                cout << tiempo << " dias, " << horas << " horas, " << minutos << " minutos y " << segundos << " segundos. ";
            }
            else {
                cout << "0 dias, " << tiempo << " horas, " << minutos << " minutos y " << segundos << " segundos. ";
            }
        }
        else {
            cout << "0 dias, 0 horas, " << tiempo << " minutos y " << segundos << " segundos.";
        }
   }
   else {
        cout << "0 dias, 0 horas, 0 minutos y " << tiempo << " segundos." ;
   } 

}
