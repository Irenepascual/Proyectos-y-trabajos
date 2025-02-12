/******************************************************************************\
 * Programación 1. Práctica 4
 * Autores: Miguel Ángel Latre y Rafael Tolosana
 * Modificado por: Irene Pascual Albericio
 * Ultima revisión: 21 de noviembre de 2020
 * Resumen: Fichero de implementación del módulo principal del programa que 
 *          manipula imágenes BMP en la 5ª práctica.
 * Nota: El código de este programa está repartido en varios módulos.
 *       Para compilarlo, hay que ejecutar el comando
 *           make bmp
 *       o, en Windows,
 *           mingw32-make bmp
 *       o ejecutar la tarea "Compilar «bmp»" de VSC.
 * 
 *       Para ejecutarlo, una vez compilado, hay que ejecutar el comando
 *           bin/bmp
 *       o, en Windows,
 *           bin\bmp.exe
 *       o ejecutar la tarea "Ejecutar «bmp»" de VSC.
\******************************************************************************/
#include <iostream>
#include <string>
#include "imagen-bmp.hpp"
using namespace std;

/* 
 * Programa de ejemplo que lee la imagen «datos/prog1.bmp» y genera la imagen
 * «datos/imagen-generada.bmp». Compílalo y ejecútalo una vez para comprobar que
 * genera la imagen «datos/imagen-generada.bmp» y que esta se puede visualizar
 * en Visual Studio Code.
 * Después modifica la función «main» (incluyendo esta especificación) para
 * resolver el problema solicitado en la tarea 2 de la práctica.
 */
int main(){                                              
    static Imagen img1;
    static Imagen img2;
    string nombreFichero;
    string nombreFicherofinal;
    char opcion;

    cout << "Escribe el nombre de un fichero BMP: ";
    cin >> nombreFichero;

    if (leerImagen(nombreFichero, img1)) {
        img2 = img1;

        cout << "Escribe una opcion: N(negativizar) o R(rotar):";
        cin >> opcion;

        while(opcion !='N' && opcion !='n' && opcion !='R' && opcion !='r'){
            cout << "Escribe una opción (N - negativizar; R - rotar): ";
            cin >> opcion;
        }

        if(opcion == 'N' || opcion == 'n'){
            for (unsigned i=0; i<(img1.alto-1); i++) {
                for (unsigned j=0; j<(img1.ancho-1); j++) {
                    (img2.pixels[i][j].rojo) = (225-img1.pixels[i][j].rojo);       
                    (img2.pixels[i][j].verde) = (225-img1.pixels[i][j].verde);
                    (img2.pixels[i][j].azul) = (225-img1.pixels[i][j].azul);
                }   
            }  
        }
        else if(opcion == 'R' || opcion == 'r'){
            for (unsigned i=0; i <(img1.alto-1); i++) {
                for (unsigned j=0; j<(img1.ancho-1); j++) {
                    img2.pixels[i][j]= img1.pixels[img1.alto-1-i][img1.ancho-1-j];
                }   
            }  
        }

        cout << "Escribe el nombre del fichero destino : ";
        cin >> nombreFicherofinal;



        guardarImagen(nombreFicherofinal, img2);
        return 0;
    }
    else {
        return 1;
    }
}