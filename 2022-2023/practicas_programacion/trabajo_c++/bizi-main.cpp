/*********************************************************************************************\
 * Programación 1. Trabajo 
 * Autores: Irene Pascual y Rubén Agustín
 * Resumen: Fichero de implementación del módulo principal que implementa el trabajo
 *          de Programación 1 del curso 2022-23.
\*********************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "estacion.hpp"
#include "nombres-ficheros.hpp"
#include "uso.hpp"
#include "usos-usuario.hpp"
#include "usuarios.hpp"

using namespace std;

const unsigned SIZE_ORDENES = 9;
const unsigned AYUDA = 0;
const unsigned FICHERO = 1;
const unsigned USOS = 2;
const unsigned ESTADISTICAS = 3;
const unsigned USUARIOS = 4;
const unsigned MAYORES = 5;
const unsigned INFORME = 6;
const unsigned DESTINOS = 7;
const unsigned FIN = 8;

const string ORDENES[SIZE_ORDENES] = {"AYUDA","FICHERO","USOS","ESTADISTICAS","USUARIO","MAYORES","INFORME","DESTINOS","FIN"};

/*
    Pre: Recibe un dato tipo string vacio

    Post: Lee el fichero de opciones y pide al usuario que ingrese una de las mostradas.
      Esta función guarda la opción elegida en la variable opción y comprueba que se pueda abrir 
      el fichero de opciones.
*/
void mostrarOpciones(string &opcion);
/*
    Pre: Tener abierto correctamente un flujo de entrada.

    Post: Muestra por pantalla el contenido de un fichero, línea a línea
*/
void mostrarFicheroEnPantalla(istream &fichero);
/*
    Pre: Recibe el nombre de la dirección de un fichero.

    Post: Comprueba si el fichero se puedo abrir, si es posible
          informa al usuario por pantalla devuelve TRUE y cierra el flujo.
          En caso contrario sacará un mensaje por el cerr y devolverá FALSE

*/
bool abrirFichero(const string direccion);
/*
    Pre: ---

    Post: Abre el flujo de un fichero de entrada, relacionado con el FICHERO_AYUDA
          y utiliza el procedimiento mostrarFicheroEnPantalla para mostrar, el fichero
          de ayuda por pantalla. Si no se puede abrir el fichero se saca un mensaje por
          el cerr de aviso al usuario.
*/
void mostrarAyuda();
/*
    Pre: Recibe una string con el contenido que se desea pasar a mayusculas.

    Post: Cambia la cadena asignada a mayusculas.
*/
void mayuscula(string &cadena);
/*
    Pre: Recibe la orden leida del teclado, que ha introducido el usuario.

    Post: Con la orden introducida busca coincidencias con las ordenes que dispone el sistema,
          en que caso de encontrar dicha coincidencia devuelve la posición correspondiente de
          la coincidencia y en caso de que no de que no exista devuelve 20, para que no coincida
          con ninguna orden establecida.
*/
unsigned consultarOrden(const string orden);
/*
    Pre: Recibe una cadena de carácteres que será modificada.

    Post: La función fichero, se encarga de mostrar la opciones de ficheros a elegir,
          reconstruir una dirección con la opción elegida y verificar si dicha opción
          se puede abrir o no.
          
          En caso de que no se pueda abrir, repetira la operación hasta que se pueda
          abrir el fichero requerido.
*/
void fichero(string &direccion);
/*
    Pre: Recibe un flujo de entrada abierto y un usuario.

    Post: Busca la id de el usuario dado en el flujo de entrada abierto,
          en caso de encontrarlo devuelve TRUE. Si no lo ha encontrado
          devuelve FALSE
*/
bool buscarId(ifstream &f,const unsigned idUsuario);

/*
    Pre: Recibe la cadena de caracteres que se quiere utilizar como fichero en uso para
         las ordenes del sistema. Además recibe la orden en mayusculas.
    
    Post: Selección la función a realizar dependiendo de la orden dada y devuelve
          un entero que será 0 si todo ha ido correctamente y en caso de que no vaya
          bien devolverá un número relacionado con el código de error correspondiente.
*/
int menuOrden(string &ficheroActivo, const string orden);
/*
    Pre: Fichero representa la dirección del fichero del que se quiere
         obtener los usos.
    
    Post: Cuenta los diferente usos del fichero activo y muestra por pantalla
          los usos como traslados, los usos como circulares y los usos totales.
          En caso de no haber podido contar los uso devuelve FALSE y si sí que
          se han podido contabilizar los usos devuelve TRUE.
*/
int usos(const string fichero);
/*
    Pre: Fichero representa la ruta de acceso al fichero del cual se quieren adquirir
         las estadisticas de los datos.
    
    Post: Obtiene la estadisticas del fichero y en caso de haber podido obtnerlas,
          las escribe por pantalla, además devuelve 0. Si no se han podido obtner 
          devuelve un -2, que es el código de error asociado a las estadisticas.
*/
int estadisticas(const string fichero);
/*
    Pre: Fichero corresponde al fichero de usuarios que se va a inspeccionar, fichero activo
         corresponde al fichero de usos seleccionado y usuario corresponde al usuario que se
         desea buscar.
    
    Post: Busca si el usuario está en el fichero de usuarios y en el fichero de usos seleccionado,
          en caso verdadero muestra los datos de dicho usuario en pantalla. En caso de no haber
          encontrado el usuario, lo comunica al usuario por pantalla.
*/
void usuarios(const string fichero, const unsigned usuario);
/*
    Pre: Fichero informe representa a la ruta del fichero que se tiene que crear, fichero estaciones
         es el fichero de donde se va a sacar la información de las esatciones del sistema y fichero
         activo es sobre el fichero del cual se va a hacer el informe

    Post: Lee el fichero estaciones, cuenta los usos de cada estación en el fichero activo, luego ordena
          las estaciones según su número de usos y si se han podido hacer todos estos pasos, se escribe el informe,
          y se muestra en pantalla la informativa de que se ha podido hacer el informe.

          En caso de que no se pueda se devuelve el codigo de error correspondiente al error. Con un mensaje
          por pantalla informativo.

*/
int informe(const string ficheroInforme,const string ficheroEstaciones,const string ficheroActivo);
/*
    Pre: Recibe una matriz rellena con las estadisticas correctamente leidas

    Post: Escribe la información contenido en la matriz estadisticas
          Ejemplo: 
          
          Distribución de los usuarios
                 |     M      F
          -------+----------------
          <=25   |   1567   1306
          26-35  |   2200   2214
          36-50  |   4686   3857
          51-65  |   3692   2326
          >65    |    848    192


*/
void escribirEstadisticas(unsigned estadisticas[][NUM_GENEROS]);
/*
    Pre: Recibe el fichero seleccionado correctamente, una matriz viajes, que no tiene
         porque estar inicializada a 0 y un vector de destinos frecuentes que tampoco 
         tiene porque estar inicializado.

    Post: Esta función va a contar los viajes que se han hecho de una estación a otra,
          seleccionando cuales de las estaciones de destino es la más frecuente para
          dicha estación de origen.

          Luego preguntará al usuario que quiere hacer con la información, si guardarla
          en un fichero o mostrala por pantalla. Si es la primera de las opciones, abrirá
          un flujo de salida asociado a la ruta del fichero dado y escribirá en él la información.
          Si por lo contrario decide mostrarlo en pantalla, simplemente mostrara dicha información
          de los destinos por la pantalla.

          La función devolverá 0 si ha ido todo bien o distinto valores de error que se relacionarán
          a un código de error asociado
*/
int ejecutarDestinos(const string ficheroActivo,unsigned viajes[][NUM_ESTACIONES],
                     unsigned destinosFrecuentes[NUM_ESTACIONES]);
/*
    Pre: Recibe el nombre del fichero del cual se quiere leer el número de usuarios.

    Post: Está función lee la cabezera y y después contea el número de lineas que hay en
          el documento para saber cuantos usuarios hay definidos. Si no se puedo abrir el
          flujo de entrada devolvera 0 usuarios.
*/
unsigned usuariosTotales(const string nombreFichero);
/*
    Pre: Recibe un vector de tipo UsosUsuario, relleno correctamente, además de
         el numero de usuarios que hay que mostrar que son los usuarios que se 
         han ordenado.

    Post: Muestra el vector dado por pantalla de tal forma que se vean cada usuario
          con sus usos y el tipo de usos que ha hecho, todo esto de forma ordenada de
          tal forma que el que más usos ha hecho sea el primero y así sucesivamente.
*/
void mostrarMayores(const UsosUsuario usuarios[], const unsigned numOrdenar);
/*
    Pre: Recibo el nombre del fichero selecionado y qu existe y el número de usuarios
         a ordenar.

    Post: Calcula el número de usuarios totales y en el fichero de usuarios e inicializa
          un vector de tipo UsosUsuario que va almacenar la información de cada usuario.
          Esta información se va a ordernar de mayor uso a menor uso y posteriormente se
          va a mostrar por pantalla.

          Si todo va bien se devolverá un 0 y si algo sale mal se devolverá un 56 denotando
          que ha habido un error al leer el fichero.
*/
int mayores(const string nombreFichero, const unsigned numOrdenar);

int main(){
    string orden = " ",ficheroActivo;
    int error = 0;
    
    fichero(ficheroActivo);
    cout << endl;
    mostrarAyuda();
    while (orden != ORDENES[FIN]){
        
        cout <<endl <<endl <<"Orden: ";
        cin >> orden;
        mayuscula(orden);
        error = menuOrden(ficheroActivo,orden);
        if(error)
            return error;
        
    }

    return 0;
}

void mostrarOpciones(string &opcion){
    
    ifstream f(FICHERO_OPCIONES);
    if (f.is_open())
    {
        cout << "Eleccion de ficheros de usos y usuarios. Opciones disponibles: " << endl;
        mostrarFicheroEnPantalla(f);        
        cout << endl << "Introduzca una opcion: ";
        cin >> opcion;

        f.close(); 
    }
    else{    
        cerr<< "No se puede abrir el fichero de entrada." << endl; 
    }

}

void mostrarAyuda(){
    
    ifstream f(FICHERO_AYUDA);
    if (f.is_open())
    {
        mostrarFicheroEnPantalla(f);   
        f.close(); 
    }
    else{    
        cerr<< "No se puede abrir el fichero "<< FICHERO_AYUDA << endl; 
    }

}

void mostrarFicheroEnPantalla(istream &fichero)
{
    string contenido;
    while( getline(fichero,contenido)){
        cout << contenido << endl;
    }
}

bool abrirFichero(const string direccion)
{
    ifstream f(direccion);
    if (f.is_open())
    {
        cout << "El fichero \"" << direccion << "\" existe y ha sido seleccionado." << endl;
        return true;
        f.close();
    }
    else{    
        cerr << "No se ha podido abrir el fichero \""<< direccion <<"\"."<< endl << endl;
        return false; 
    }

}

void mayuscula(string &cadena)
{
    for (unsigned i = 0; i < unsigned(cadena.length()); i++) {

		cadena[i] = toupper(cadena[i]);
	}
}

unsigned consultarOrden(const string orden)
{
    for(unsigned i = 0; i < SIZE_ORDENES; i++)
    {
        if(orden == ORDENES[i])
        {
            return i;
        }
    }

    return 20;
}

void fichero(string &direccion)
{
    string opcion;
    bool estado = false;
    do 
    {
        mostrarOpciones(opcion);
        direccion = construirNombreFicheroUsos(opcion);
        estado = abrirFichero(direccion);
        
    }
    while(!estado);
}

bool buscarId(ifstream &f,const unsigned idUsuario)
{
    unsigned id;
    string contenido;
    getline(f,contenido);
    while(f >> id)
    {
        
        if( idUsuario == id)
        {
           return true;
        }
        getline(f,contenido);
        
    }
    return false;
}

int usos(const string ficheroActivo)
{
    unsigned traslado = 0, circular = 0;
    cout << "Fichero de usos seleccionado actualmente: \""<< ficheroActivo <<"\"" << endl;
    if(contarUsos(ficheroActivo,traslado, circular))
    {
        cout << "Número de usos como traslado: "<< right <<setw(8) << traslado << endl;
        cout << "Número de usos circulares:    "<<right <<setw(8) << circular << endl;
        cout << "Número de usos como totales:  "<< right << setw(8) << traslado+circular << endl;
        return 0;
    }
    else
    {
        cerr << " Error al contar los usos del fichero: \""<< ficheroActivo <<"\""<< endl;
        return -1;
    }
}

void escribirEstadisticas(unsigned estadisticas[][NUM_GENEROS])
{
    cout << "Distribución de los usuarios"<<endl;
    cout<<setw(8)<<"|"<< setw(6) << "M"<<setw(7)<<"F"<< endl;
    cout<<setfill('-') << setw(8) << "+" <<setw(16)<<"" <<endl;
    for(unsigned i = 0; i< NUM_EDADES; i++)
    {
        cout<<setfill(' ')<<left<<setw(6)<<RANGO_EDADES[i]<<" |";
        for(unsigned j = 0; j < NUM_GENEROS; j++)
        {
            cout <<right<<setw(7)<< estadisticas[i][j];
        }
        cout << endl;
    }  
}

int estadisticas(const string fichero)
{
    unsigned estadisticas[NUM_EDADES][NUM_GENEROS]={0};
    if(obtenerEstadisticas(fichero,estadisticas))
    {
        escribirEstadisticas(estadisticas);
        return 0;
    }
    else
    {
        cerr << "Error al sacar las estadisticas del fichero: "<<fichero<< endl;
        return -2;
    }
}

void usuarios(const string fichero, const unsigned usuario)
{
    string edad , genero;
    if(buscarUsuario(fichero,usuario,genero,edad))
    {
        if(genero == "F")
        {
            cout << "La usuaria "<< usuario;
        }
        else if(genero == "M")
        {
            cout << "El usuario "<< usuario;
        }
        else{
            cout<< "El/La usuario/a "<< usuario;
        }

        cout << " está en el rango de edad \"" << RANGO_EDADES[indiceRangoEdad(edad)] <<"\"."<<endl;

    }
    else
    {
        cerr << "El/la usuario/a "<< usuario<<" no aparece en el fichero \""<<FICHERO_USUARIOS<<"\"." << endl;
    }

}

int informe(const string ficheroInforme,const string ficheroEstaciones,const string ficheroActivo)
{
    Estacion estaciones[NUM_ESTACIONES];
                
            
    if(leerEstaciones(ficheroEstaciones,estaciones) )
    {
        
        if(contarUsosEstaciones(ficheroActivo,estaciones))
        {
            ordenarPorUso(estaciones);
            if(escribirInformeEstaciones(ficheroInforme,estaciones))
            {
                cout << "Informe \"" << ficheroInforme <<"\" creado correctamente."<< endl;
                return 0;
            }
            else
            {
                cerr << "No se pudo crear el informe \""<<ficheroInforme << "\"." << endl;
                return -7;
            }
            
        }
        else
        {
            cerr << "Error al contar" << endl;
            return -5;
        }
        
        
    }
    else
    {
        cerr << "No se pudo abrir el fichero\"" << ficheroEstaciones<< "\"."  << endl;
        return -4;
    }

}

int ejecutarDestinos(const string ficheroActivo,unsigned viajes[][NUM_ESTACIONES], unsigned destinosFrecuentes[NUM_ESTACIONES])
{
    if(contarViajesOrigenDestino(ficheroActivo,viajes))
        calcularDestinosMasFrecuentes(viajes,destinosFrecuentes);
    else
    {
        cerr << "Error al abrir el fichero\""<<ficheroActivo<<"."<<endl;
        return -9;
    }
        
    string ficheroDestinos,vacio;
    cout <<"Escriba el nombre del fichero del informe" <<endl
         << "(presione solo ENTRAR para escribirlo en la pantalla): ";
    getline(cin,vacio);
    getline(cin,ficheroDestinos);
    if(ficheroDestinos != "")
    {
        ofstream destinos(ficheroDestinos);
        if(destinos.is_open())
        {
            escribirInformeDestinos(destinos,viajes,destinosFrecuentes);
            cout << "Infome \""<< ficheroDestinos<<"\" creado correctamente." << endl;
            destinos.close();
        }
        else
        {
            cerr << "Error al crear el fichero de texto: "<<ficheroDestinos << endl;
            return -10;
        }
    }
    else
    {
        escribirInformeDestinos(cout,viajes,destinosFrecuentes);
        
    }
    return 0;
}

unsigned usuariosTotales(const string nombreFichero)
{
    ifstream usuarios(nombreFichero);
    string contenido;
    unsigned nUsuarios=0;
    if(usuarios.is_open())
    {
        getline(usuarios,contenido);
        while(getline(usuarios,contenido))
        {
            nUsuarios++;
        }
        
    }
    
    return nUsuarios;
}

void mostrarMayores(const UsosUsuario usuarios[], const unsigned numOrdenar)
{
    char tabla = '=';
    cout << " "<< setw(9) << "Usuario"<<" "<<setw(9)
         << "Traslados" <<" "<<setw(9)<<"Circular"
         << " "<< setw(9) <<"Total"<<endl;
    for(unsigned i = 0; i <4; i++)
    {
        cout<<" "<<setfill(tabla)<<setw(9)<<"";
    }
    cout <<setfill(' ')<<endl;
    for(unsigned i = 0; i < numOrdenar; i++)
    {
        if(usuarios[i].id != 0)
        {
            cout << " "<<setw(9) << usuarios[i].id
                 << " "<<setw(9) << usuarios[i].traslados
                 << " "<<setw(9) << usuarios[i].circulares
                 << " "<<setw(9) << numUsosTotales(usuarios[i])<<endl;
        }
        
    }
}

int mayores(const string nombreFichero, const unsigned numOrdenar)
{
    unsigned numUsuarios,numUsuariosTotales = usuariosTotales(FICHERO_USUARIOS);
    
    UsosUsuario usuarios[numUsuariosTotales];
    if(obtenerUsosPorUsuario(nombreFichero,usuarios,numUsuarios))
    {
        cout << "Número usuarios distintos: " << numUsuarios << endl << endl;
        ordenar(usuarios,numUsuarios,numOrdenar);
        mostrarMayores(usuarios,numOrdenar);
        return 0;
    }
    else 
    {
        return 56;
    }
}

int menuOrden(string &ficheroActivo, const string orden)
{
    int diagnosis = 0;
    switch (consultarOrden(orden))
        {
            case AYUDA:
            {
                cout<< endl;
                mostrarAyuda();
                diagnosis = 0;
            }
            break;

            case FICHERO:
            {
                fichero(ficheroActivo);
                diagnosis = 0;
            }
            break;

            case USOS:
            {
                diagnosis = usos(ficheroActivo);
            }      
            break;

            case ESTADISTICAS:
            {            
                diagnosis = estadisticas(FICHERO_USUARIOS);
            }          
            break;

            case USUARIOS:
            {
                unsigned idUsuario;
                cin >> idUsuario;
                usuarios(FICHERO_USUARIOS,idUsuario);
                diagnosis = 0;
            }
            break;

            case MAYORES:
            {
                unsigned numOrdenar;
                cin >> numOrdenar;
                diagnosis = mayores(ficheroActivo,numOrdenar);
                

            }         
            break;

            case INFORME:
            {
                string ficheroInforme;
                cin >> ficheroInforme;
                diagnosis = informe(ficheroInforme,FICHERO_ESTACIONES,ficheroActivo);
                
            }
            break;

            case DESTINOS:
            {
                unsigned viajes[NUM_ESTACIONES][NUM_ESTACIONES],destinosFrecuentes[NUM_ESTACIONES];
                diagnosis = ejecutarDestinos(ficheroActivo,viajes,destinosFrecuentes);
                
            }
            break;

            case FIN:
            break;

            default:

                cerr << "La orden \"" << orden << "\" desconocida" << endl ;
                return 0;
            
            
        }
    return diagnosis;
}
