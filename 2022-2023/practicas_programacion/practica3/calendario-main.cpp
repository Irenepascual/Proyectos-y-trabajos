/******************************************************************************\
 * Curso de Programación 1. Práctica 3
 * Autores: ¡PON AQUÍ TU NOMBRE!
 * Última revisión: 21 de octubre de 2021
 * Resumen: Programa que escribe en la pantalla el calendario de un determinado
 *          mes de un año, que se solicitan al usuario. El programa se asegura 
 *          de que el mes introducido está comprendido entre 1 y 12 y de que el 
 *          año es igual o posterior a 1900.
 *          Clase de problemas correspondiente al tema 7 (diseño modular y
 *          descendente) y 3ª práctica.
 * Nota: Leer el fichero README.md, que contiene instrucciones para su 
 *       compilación y ejecución.
\******************************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include <fechas.hpp>

using namespace std;

/*
 * Pre:  1 ≤ mes ≤ 12
 * Post: Ha escrito en la pantalla el nombre (en mayúsculas) del mes
 *       correspondiente al valor del parámetro «mes».
 * Nota: Este código lo podrás comprender adecuadamente cuando hayamos visto
 *       los temas 9 (vectores) y 10 (cadenas de caracteres).
 */
void escribirNombreMes(unsigned mes) {
    const string NOMBRES_MES[12] = { "ENERO",
            "FEBRERO", "MARZO", "ABRIL", "MAYO", "JUNIO", "JULIO", "AGOSTO",
            "SEPTIEMBRE", "OCTUBRE", "NOVIEMBRE", "DICIEMBRE" };
    cout << NOMBRES_MES[mes - 1];
}

/*
 * Pre: 1 ≤ mes ≤ 12
 * Post: Se pide al usuario que escriba un número del mes, obligatoriamente 
 *       entre el mes 1 y el 12 (si no lo hiciese, lo sigue pidiendo hasta que
 *       el usuario lo haga).
 */
void pedirMes (int& mes){
    cout << "Introduce un mes del 1 al 12:";
    cin >> mes;
    while (mes<1 || mes>12){
        cout << "El mes debe estar comprendido entre 1 y 12:";
        cin >> mes;
    }
}

/*
 * Pre:  año>1900
 * Post: Se pide al usuario que escriba un año, obligatoriamente
 *       a partir del año 1900 (si no lo hiciese, lo sigue pidiendo hasta que
 *       el usuario lo haga).
 */
void pedirAgno (int& agno){
    cout << "Introduce un año posterior a 1900:";
    cin >> agno;
    while (agno<1900){
        cout << "El año debe ser posterior a 1900:";
        cin >> agno;
    }
}

/*
 * Pre:----
 * Post: muestra la cabecera, con el año primero, y después los días de la semana.
 */
void escribirCabecera(int& mes, int& agno){
    escribirNombreMes(mes);
    cout << right << agno << endl;
    cout << setw(4) << "L"
         << setw(4) << "M"
         << setw(4) << "X"
         << setw(4) << "J"
         << setw(4) << "V"
         << setw(4) << "S"
         << setw(4) << "D"
         << endl;
    cout << "----------------------------" ;
}

/*
 * Pre:  
 * Post: se muestra por semanas el calendario.
 */
void presentarCalendario(int mes, int agno){
    int calendario=0;
    for(int semana=0; semana<7; semana++){
        for(int i=1; i<8; i++){
            calendario=(semana)*7+i-diaDeLaSemana(1 , mes, agno);
            if(calendario<=0){
                cout << " ";
            }
            else if(calendario<=diasDelMes(mes, agno)){
                cout << setw(4) << calendario;
            }
        }
        cout << endl;
    } 
}

/*
 * Pre:  
 * Post: pide el mes y el año y a partir de esos datos, 
 *       crea el calendario atendiendo a las instrucciones de
 *       escribir la cabecera y presentar las semanas del mes.
 */
int main() {
    int mes;
    int agno;
    pedirMes(mes);
    pedirAgno(agno);
    escribirCabecera(mes, agno);
    presentarCalendario(mes, agno);
    return 0;
}
