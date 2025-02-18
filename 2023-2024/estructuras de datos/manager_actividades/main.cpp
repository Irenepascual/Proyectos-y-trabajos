//-------------------------------------------------------------------------------------------
// File:   main.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------

#include "actividad.hpp"
#include "periodo.hpp"
#include "instante.hpp"
#include "coleccionSeg.hpp"
#include <fstream>
#include <iomanip>

//Pre: Se le asigna un flujo de entrada, asi como un instante i.
//Post: La función lee una línea del fichero de entrada, y a partir de los
//      valores leídos, procede a crear un instante. Si los datos leidos son válidos, devuelve true (se ha 
//      creado el instante correctamente) y en caso contrario false.
bool lecturaFecha(ifstream &f_ent, instante &i){
    string minuto, hora, dia, mes, anyo;
    getline(f_ent,anyo,'-');
    getline(f_ent,mes,'-');
    getline(f_ent,dia,' ');
    getline(f_ent,hora,':');
    getline(f_ent,minuto);

   if (crearInstante(stoi(dia),stoi(mes),stoi(anyo),stoi(hora),stoi(minuto),i)){
        return true;
   }
   else{
        return false;
   }

}

//Pre: Recibe un flujo de entrada y una actividad a.
//Post: La función lee una linea del fichero de entrada y a partir de los
//      datos leídos, procede a crear una actividad. Si los valores leidos son validos, devuelve true (se ha 
//      creado la actividad correctamente), y en caso contrario false.
void lecturaActividad(ifstream &f_ent, actividad &a){
    string nombre, tEstimado, descripcion;
    getline(f_ent,nombre);
    getline(f_ent,tEstimado);
    getline(f_ent,descripcion);

    crearActividad(nombre,descripcion,stod(tEstimado),a);

}

bool crearInstantesPeriodo(ifstream &f_ent, ofstream &f_sal, instante &inicio, instante &final, periodo &p){
    if(lecturaFecha(f_ent,inicio)){
        if(lecturaFecha(f_ent,final)){
            if(crearPeriodo(inicio,final,p)){
                return true;
            }
            else{
                f_sal << "PERIODO NO VALIDO" << endl;
                return false;
            }
        }
        else{
            f_sal << "INSTANTE NO VALIDO" << endl;
            f_sal << "PERIODO NO VALIDO" << endl;
        return false;
        }
    }
    else{
        f_sal << "INSTANTE NO VALIDO" << endl;
        f_sal << "PERIODO NO VALIDO" << endl;
        return false;
    }
}

//Pre: Recibe un flujo de salida, un instante inicial y otro final.
//Post: Escribe por pantalla un periodo a partir de los dos instantes en un formato correcto.
void escribir(ofstream &f_sal, instante inicio, instante final){
    f_sal << anyo(inicio) << "-" << setw(2) << setfill('0') << mes(inicio) << "-" << setw(2) << setfill('0') << dia(inicio) << " " << setw(2) << setfill('0') << hora(inicio) << ":" << setw(2) << setfill('0') << minuto(inicio);
    f_sal << " ;;; " << anyo(final) << "-" << setw(2) << setfill('0') << mes(final) << "-" << setw(2) << setfill('0') << dia(final) << " " << setw(2) << setfill('0') << hora(final) << ":" << setw(2) << setfill('0') << minuto(final);
}

int main(){
    //definimos
    instante inicio;
    instante final;
    periodo p;
    actividad a;
    string nombre;
    string nombre_actividad;
    //crea la lista enlazada y la inicializa
    coleccionSeg<periodo,actividad> colSeg;
    crear(colSeg);
    //se procede a intentar abrir el fichero de entrada
    ifstream f_ent("entrada.txt");
    if(f_ent.is_open()){
        //se procede a intentar abrir el fichero de salida
        ofstream f_sal("salida.txt");
        if(f_sal.is_open()){
            string instruccion;
            //variable auxiliar para pasar de linea 
            string salto;
            //mientras que no se termine el fichero de entrada, realizamos las funciones
            while (f_ent >> instruccion) {
                getline(f_ent,salto);
                if (instruccion == "A") {
                    //si se puede crear los instantes y el periodo correctamente, realizamos la instruccion A.
                    if(crearInstantesPeriodo(f_ent,f_sal,inicio,final,p)){
                        lecturaActividad(f_ent, a);
                        //si no se ha podido añadir el periodo, sacamos por pantalla que no ha sido introducido.
                        if(!agnadir(colSeg, p, a)){
                            f_sal << "NO ";
                        }
                        f_sal << "INTRODUCIDO: [ ";
                        escribir(f_sal, inicio, final);
                        f_sal << " ]" << " ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " << obtenerDescripcion(a) << endl;
                    }

                } else if (instruccion == "C") {
                     //si se puede crear los instantes y el periodo correctamente, realizamos la instruccion C
                    if(crearInstantesPeriodo(f_ent,f_sal,inicio,final,p)){
                        lecturaActividad(f_ent, a);
                        //si no se ha podido cambiar la actividad, lo hacemos saber escribiendolo por pantalla,
                        if (!actualizar(colSeg, p, a))
                        {
                            f_sal << "NO ";
                        }
                        f_sal << "CAMBIADO: [ " ; 
                        escribir(f_sal, inicio, final);
                        f_sal << " ] ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " << obtenerDescripcion(a)<<endl;
                    }
                
                } else if (instruccion == "O"){
                    //si se puede crear los instantes y el periodo correctamente, realizamos la instruccion O
                    if(crearInstantesPeriodo(f_ent,f_sal,inicio,final,p)){
                        //si no existe el periodo buscado en el listado, lo hacemos saber por pantalla.
                        if(!obtener(colSeg, p, a)){
                            f_sal << "NO LOCALIZADO : [";
                            escribir(f_sal,inicio,final); 
                            f_sal << " ]" << endl;
                            }
                            else{
                                f_sal << "LOCALIZADO : [" ;
                                escribir(f_sal, inicio,final);
                                f_sal << " -> " << obtenerDuracion(p) << " ] ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " << obtenerDescripcion(a) << endl;  
                            }
                    }

                } else if (instruccion == "S"){
                    //si se puede crear los instantes y el periodo correctamente, realizamos la instruccion S
                    if(crearInstantesPeriodo(f_ent,f_sal,inicio,final,p)){
                        //si no se solapa, lo hacemos saber por pantalla
                        if (!seSolapa(colSeg, p)){
                            f_sal << "NO ";
                        }
                        f_sal << "SOLAPE con: [ " ;
                        escribir(f_sal, inicio, final);
                        f_sal << " ]" << endl;                               
                    }
                
                } else if (instruccion == "B") {
                    //si se puede crear los instantes y el periodo correctamente, realizamos la instruccion B
                    if(crearInstantesPeriodo(f_ent,f_sal,inicio,final,p)){
                        //si no lo podemos borrar, lo hacemos saber por pantalla.
                        if(!borrar(colSeg,p)){
                            f_sal << "NO ";
                        }
                        f_sal << "BORRADO: [";
                        escribir(f_sal,inicio,final);
                        f_sal << " ]" << endl;
                    }
                            
                } else if (instruccion == "LA"){
                    int total = 0;
                    //inicializa el iterador
                    iniciarIterador(colSeg);
                    //obtenemos la actividad de la que queremos saber los datos
                    getline(f_ent, nombre_actividad);
                    f_sal << "****ACTIVIDAD: " << nombre_actividad << endl;
                    //Mientras sigan habiendo actividades disponibles, se recorren para buscar el numero de actividades
                    //que coinciden con la actividad dada
                    do{
                        if(siguienteVal(colSeg,a)){
                            if(nombre_actividad == obtenerNombre(a)){
                                //obtenemos el periodo de esas actividades que coinciden con la que buscabamos
                                if(siguienteSegm(colSeg,p)){
                                    inicio = obtenerInicio(p);
                                    final = obtenerFinal(p);
                                    f_sal << "[ ";
                                    escribir(f_sal,inicio,final);
                                    f_sal << " -> " << obtenerDuracion(p) << " ]" << endl;
                                    total = total + obtenerDuracion(p);
                                }
                            }
                        }
                    }while(avanza(colSeg));
                    //escribe el total
                    f_sal << "*****TOTAL dedicados: " << total  << endl;
                } else if (instruccion == "LT"){
                    f_sal << "-----LISTADO: " << tamagno(colSeg) << endl;
                    //inicializa el iterador
                    iniciarIterador(colSeg);
                    //mientras sigan habiendo datos disponibles en la lista enlazada, obtendremos sus datos para escribirlos
                    //en el fichero de salida
                    do{
                        if(siguienteSegm(colSeg,p)){
                            if(siguienteVal(colSeg,a)){
                                inicio = obtenerInicio(p);
                                final = obtenerFinal(p);
                                f_sal << "[ " ;
                                escribir(f_sal, inicio, final);
                                f_sal << "->" << obtenerDuracion(p) << " ] ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " << obtenerDescripcion(a) << endl;                  
                            }
                        }
                    }while(avanza(colSeg));
                    f_sal << "-----" << endl;
                }
            }
            //cerramos fichero de salida
            f_sal.close();
        }
        //cerramos fichero de entrada
        f_ent.close();
    }
    else{
        //comentario de error si no se ha podido abrir correctamente el fichero
        cerr << "Error al abrir el fichero" <<endl;
    }
    
}

