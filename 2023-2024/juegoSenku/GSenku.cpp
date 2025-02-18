// Autores: Irene  Pascual Albericio  ; Sergio Garcés Maluenda      

#include <iostream>
#include <fstream> 
#include <unistd.h>
#include "GSenku.hpp"

using namespace std;


// FUNCIONES / PROCEDIMIENTOS 

// Pre: true
// Post: lee la configuración y el estado del tablero del fichero de configuración que se le pasa como argumento
//      inicializando tablero y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarTablero(const string nombreFichero, tpTablero &tablero){
    ifstream configuracion;
    configuracion.open(nombreFichero);    //Abro el fichero de configuración

    if (!configuracion.is_open()) {       // Si no lo puedo abrir, return FALSE
        return false;
    }

    configuracion >> tablero.nfils >> tablero.ncols;   //leo las dimensiones de la matriz

    for (int i = 0; i < MAXDIM; i++) {           // inicializo todo el tablero a NO_USADA
        for (int j = 0; j < MAXDIM; j++) {
            tablero.matriz[i][j] = NO_USADA;
        }
    }

    string linea;
    getline(configuracion, linea); // Descartamos la línea que contiene las dimensiones

    for (int i = 0; i < tablero.nfils; i++) {       //Leo el estado de cada celda desde el fichero de configuración
        getline(configuracion, linea);

        int idx = 0;
        for (int j = 0; j < tablero.ncols; j++) {
            char estadoCelda = linea[idx];

            if (estadoCelda == '-') {               // Si el estado de celda es '-'
                tablero.matriz[i][j] = NO_USADA;
            } else if (estadoCelda == 'x') {        // Si el estado de celda es 'x'
                tablero.matriz[i][j] = VACIA;
            } else if (estadoCelda == 'o') {        // Si el estado de celda es 'o'
                tablero.matriz[i][j] = OCUPADA;
            } else {
                configuracion.close();
                return false;
            }

            idx += 2; // Incrementamos el índice en 2 para saltar el espacio entre los caracteres de estado en la línea
        }
    }
    configuracion.close();
    return true;
}





// Pre: true 
// Post: lee los movimientos válidos del fichero que se le pasa como argumento 
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos) {
    ifstream direcciones;
    direcciones.open(nombreFichero);

    // Si no se puede abrir el archivo, devuelve false
    if (!direcciones.is_open()) {
        return false;
    }

    // Inicializa todos los movimientos como no válidos
    for (int i = 0; i < 8; i++) {
        movimientos.validos[i] = false;
    }

    string linea;
    int numLinea = 0;
    while (getline(direcciones, linea)) {  //Mientras podamos leer líneas
        for (int i = 0; i <= 4; i+=2) {  //Recorremos la línea (+2 porque hay espacios)
            char caracter = linea[i];

            // Si el caracter es '+', marca el movimiento como válido
            if (caracter == '+') {
                if (numLinea == 0) {
                    if (i == 0) {
                        movimientos.validos[superiorIzquierda] = true;  // Si hay x arriba a la izquierda, se puede ir ahi
                    } else if (i == 2) {
                        movimientos.validos[superior] = true;  // Si hay x arriba en medio, se puede ir ahi
                    } else if (i == 4) {
                        movimientos.validos[superiorDerecha] = true;  // Si hay x arriba a la derecha, se puede ir ahi
                    }
                } else if (numLinea == 1) {
                    if (i == 0) {
                        movimientos.validos[izquierda] = true;  // Si hay x a la izquierda, se puede ir ahi
                    } else if (i == 4) {
                        movimientos.validos[derecha] = true;   // Si hay x a la derecha, se puede ir ahi
                    }
                } else if (numLinea == 2) {
                    if (i == 0) {
                        movimientos.validos[inferiorIzquierda] = true;  // Si hay x abajo a la izquierda, se puede ir ahi
                    } else if (i == 2) {
                        movimientos.validos[inferior] = true;   // Si hay x abajo en medio, se puede ir ahi
                    } else if (i == 4) {
                        movimientos.validos[inferiorDerecha] = true;  // Si hay x abajo a la derecha, se puede ir ahi
                    }
                }
            }
        }
        numLinea++;  //Avanzamos a la siguiente línea
    }

    direcciones.close();
    return true;
}


// Pre: tablero contiene el estado actual de la ejecución de la búsqueda de la solución
// Post: Se ha mostrado el tablero por pantalla
void mostrarTablero(const tpTablero & tablero){
    for (int i = 0; i < tablero.nfils; i++) {
        for (int j = 0; j < tablero.ncols; j++) {
            tpEstadoCelda estadoCelda = tablero.matriz[i][j];

            if (estadoCelda == NO_USADA) {
                cout <<  "\033[31;40m" << "-" << "\033[0m";  //Muestra NO_USADA de color rojo
            } else if (estadoCelda == VACIA) {
                cout << "\033[34;40m" << "x" << "\033[0m";  // Muestra VACIA de color azul
            } else if (estadoCelda == OCUPADA) {
                cout << "\033[33;40m" << "o" << "\033[0m";  // Muestra OCUPADA de color amarillo
            } 
            cout << " ";
        }
        cout << endl;
    }
}

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movimientosValidos contiene los movimientos que están permitidos, 
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo) {
    int piezasRestantes = contarPiezas(tablero);

    if (piezasRestantes == 1) { // Caso base: solo queda una pieza en el tablero, hemos encontrado una solución
        return 1;
    }

    for (int i = 0; i < tablero.nfils; i++) {
        for (int j = 0; j < tablero.ncols; j++) {
            if (tablero.matriz[i][j] == OCUPADA) {      // Si hay una pieza
                tpPosicion origen = {i, j};             // Le asigna su direccion de origen

                for (int d = 0; d < 8; d++) { //Comprueba los 8 movimientos posibles
                    if (movValidos.validos[d]) { // Comprobamos cada dirección si está inicializada como posible 
                        tpPosicion destino = {i,j};   //Asigna las posiciones de destino según el movimiento        
                        if(d==0){
                        destino = {i-2,j-2};
                        }
                        if(d==1){
                        destino = {i-2,j};
                        }
                        if(d==2){
                        destino = {i-2,j+2};
                        }
                        if(d==3){
                        destino = {i,j-2};
                        }                
                        if(d==4){
                        destino = {i,j+2};
                        }          
                        if(d==5){
                        destino = {i+2,j-2};
                        } 
                        if(d==6){
                        destino = {i+2,j};
                        }  
                        if(d==7){
                        destino = {i+2,j+2};
                        }  

                        if (destino.x >= 0 && destino.x < tablero.nfils && destino.y >= 0 && destino.y < tablero.ncols) { // Si el destino esta en los limites del tablero
                            if (esMovimientoValido(tablero, origen, destino, movValidos)) {  // Si puede realizar ese movimiento
                                tpMovimientoPieza movimiento = {origen, destino};

                                // Realiza el movimiento y lo añade a la solución parcial
                                realizarMovimiento(tablero, movimiento);
                                solucionParcial.movs[solucionParcial.numMovs++] = movimiento;

                                // Mostrar el tablero con retardo
                                if (retardo > 0) {
                                mostrarTablero(tablero);
                                cout << movimiento.origen.x << "," << movimiento.origen.y << ":" << movimiento.destino.x << "," << movimiento.destino.y << endl;
                                cout << endl;
                                usleep(retardo*1000);   
                                }

                                // Sigue busando solucion hasta que la haya mediante la llamada recursiva
                                if (buscaSolucion(tablero, movValidos, solucionParcial, retardo) == 1) {
                                    return 1;
                                }

                                // Si la llamada recursiva no ha encontrado una solución, deshacemos el movimiento y lo quitamos de la solución parcial
                                deshacerMovimiento(tablero, movimiento);
                                solucionParcial.numMovs--;
                                // Mostrar el tablero con retardo
                                if (retardo > 0) {
                                 mostrarTablero(tablero);
                                cout << movimiento.destino.x << "," << movimiento.destino.y << ":" << movimiento.origen.x << "," << movimiento.origen.y << endl;
                                cout << endl;
                                usleep(retardo*1000);                      //funcion de retardo
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0; // Si no hemos encontrado una solución, devolvemos 0
}




// Pre: listaMovimientos contiene la lista de movimientos con la solucion 
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el 
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos(const string nombreFichero, const tpListaMovimientos &solucion) {
    ofstream resultado;
    resultado.open(nombreFichero);

    if (solucion.numMovs == 0) { // Si la lista de movimientos está vacía
        resultado << "-1" << endl; // Escribe -1 en el fichero
    } else {
        // Recorre la lista de movimientos y escribe cada movimiento en una línea
        for (int i = 0; i < solucion.numMovs; i++) {
            tpMovimientoPieza movimiento = solucion.movs[i];
            resultado << movimiento.origen.x << "," << movimiento.origen.y << ":";
            resultado << movimiento.destino.x << "," << movimiento.destino.y << endl;
        }
    }

    resultado.close();
}

// Pre: 'tablero' debe ser un objeto tpTablero válido, con una matriz de tamaño nxm.
//      'origen' y 'destino' deben ser objetos tpPosicion válidos, con coordenadas x e y dentro de los límites de la matriz del tablero.
//      'movValidos' debe ser un objeto tpMovimientosValidos válido, con una lista de movimientos válidos.
// Post: Devuelve 'true' si el movimiento entre 'origen' y 'destino' es válido según 'movValidos' y el estado actual del 'tablero'.
bool esMovimientoValido(const tpTablero &tablero, const tpPosicion &origen, const tpPosicion &destino, const tpMovimientosValidos &movValidos) {
    tpDireccion direccion;
    int dx = destino.x - origen.x;
    int dy = destino.y - origen.y;
    //Adjudica a cada destino su movimiento
    if (dx == -2 && dy == -2) {
        direccion = superiorIzquierda;
    } else if (dx == -2 && dy == 0) {
        direccion = superior;
    } else if (dx == -2 && dy == 2) {
        direccion = superiorDerecha;
    } else if (dx == 0 && dy == -2) {
        direccion = izquierda;
    } else if (dx == 0 && dy == 2) {
        direccion = derecha;
    } else if (dx == 2 && dy == -2) {
        direccion = inferiorIzquierda;
    } else if (dx == 2 && dy == 0) {
        direccion = inferior;
    } else if (dx == 2 && dy == 2) {
        direccion = inferiorDerecha;
    } else {
        return false;
    }

    if (!movValidos.validos[direccion]) {
        return false;
    }

    tpPosicion intermedia;
    intermedia.x = origen.x + dx / 2;
    intermedia.y = origen.y + dy / 2;

    // Comprobamos que el movimiento siga las reglas del juego de salir de una posicion ocupada, saltando otra ocupada, y cayendo en una libre
    if (tablero.matriz[origen.x][origen.y] != OCUPADA || tablero.matriz[intermedia.x][intermedia.y] != OCUPADA || tablero.matriz[destino.x][destino.y] != VACIA) {
        return false;
    }

    return true;
}

// Pre: 'tablero' debe ser un objeto tpTablero válido, con una matriz de tamaño nxm.
//      'movimiento' debe ser un objeto tpMovimientoPieza válido, con posiciones de origen y destino dentro de los límites de la matriz del tablero.
// Post: Realiza el movimiento especificado por 'movimiento' en el 'tablero', actualizando su estado.
void realizarMovimiento(tpTablero &tablero, const tpMovimientoPieza &movimiento) {
    tpPosicion intermedia;
    intermedia.x = (movimiento.origen.x + movimiento.destino.x) / 2;
    intermedia.y = (movimiento.origen.y + movimiento.destino.y) / 2;

    tablero.matriz[movimiento.origen.x][movimiento.origen.y] = VACIA;
    tablero.matriz[intermedia.x][intermedia.y] = VACIA;
    tablero.matriz[movimiento.destino.x][movimiento.destino.y] = OCUPADA;
}

// Pre: 'tablero' debe ser un objeto tpTablero válido, con una matriz de tamaño nxm.
//      'movimiento' debe ser un objeto tpMovimientoPieza válido, con posiciones de origen y destino dentro de los límites de la matriz del tablero.
// Post: Deshace el movimiento especificado por 'movimiento' en el 'tablero', restaurando su estado previo al movimiento.
void deshacerMovimiento(tpTablero &tablero, const tpMovimientoPieza &movimiento) {
    tpPosicion intermedia;
    intermedia.x = (movimiento.origen.x + movimiento.destino.x) / 2;
    intermedia.y = (movimiento.origen.y + movimiento.destino.y) / 2;

    tablero.matriz[movimiento.origen.x][movimiento.origen.y] = OCUPADA;
    tablero.matriz[intermedia.x][intermedia.y] = OCUPADA;
    tablero.matriz[movimiento.destino.x][movimiento.destino.y] = VACIA;
}

//Pre: 'tablero' debe ser un objeto tpTablero válido, con una matriz de tamaño nxm
//Post: Devuelve el número total de piezas en el 'tablero' con el valor OCUPADA
int contarPiezas(const tpTablero &tablero) {
    int contador = 0;
    for (int i = 0; i < tablero.nfils; i++) {
        for (int j = 0; j < tablero.ncols; j++) {
            if (tablero.matriz[i][j] == OCUPADA) contador++;
        }
    }
    return contador;
}
