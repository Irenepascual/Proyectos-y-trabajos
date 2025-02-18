//-------------------------------------------------------------------------------------------
// File:   periodo.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------


#include "periodo.hpp"
#include "instante.hpp"

//Pre: La funcion recibe un instante inicial, uno final, y un periodo.
//Post: Dado un instante de tiempo i y un instante f igual o posterior a i, devuelve un periodo
//      cronológicamente correcto y finalizado, igual al resultante de: iniciarse en i y finalizar en f.
//      Parcial: la operación no está definida si los instantes i y f no pueden constituir un periodo 
//      cronológicamente correcto, es decir, el instante f es anterior al instante i, o bien no pertenecen 
//      al mismo año y mes (PeriodoNoVálido).
bool crearPeriodo(instante inicio,  instante final, periodo &p){
    if(anyo(inicio)==anyo(final)){
        if((inicio == final) || anterior1(inicio,final)){
            p.inicio= inicio;
            p.final= final;
            return true;
        }
        return false;
    }    
    return false;
}

//Pre: La funcion recibe un periodo p
//Post: devuelve el instante inicial del periodo p.
instante obtenerInicio(periodo &p){
    return p.inicio;
}

//Pre: La funcion recibe un periodo p
//Post: devuelve el instante final del periodo p.
instante obtenerFinal(periodo &p){
    return p.final;
}

//Pre: La funcion recibe un periodo p
//Post: devuelve un natural correspondiente a la duracion del periodo p(diferencia entre instante inicial y final).
int obtenerDuracion(periodo &p){
    return diferencia(p.inicio, p.final);
}

//Pre: la función recibe dos periodos p.
//Post: Devuelve true si ambos periodos son iguales, y false en caso contrario.
bool iguales(const periodo &p1, const  periodo &p2){
    if ((p1.inicio == p2.inicio) && (p1.final == p2.final)){
        return true;
    }
    else{
        return false;
    }
}

//Pre: la función recibe dos periodos p.
//Post: Devuelve true si el periodo p1 es anterior al periodo p2, y false en caso contrario.
bool anterior(const periodo &p1, const  periodo &p2){
    if (anterior1(p1.inicio, p2.inicio)){
        return true;
    }
    if((p1.inicio == p2.inicio)){
        if(anterior1(p1.final, p2.final)){    
            return true;
        }
    }
    else{
        return false;
    }
}

//Pre: la función recibe dos periodos p.
//Post: devuelve verdad si y solo si ambos periodos de tiempo se 
//      solapan: ambos periodos son iguales o, incluyen instantes de tiempo en común distintos de los 
//      respectivos inicio y final (es decir, la intersección de los periodos como intervalos abiertos 
//      no es vacía, si el inicio de un periodo es igual al final del otro periodo no hay solape).}
bool solapados(const periodo &p1, const periodo &p2){
    if ((p1.inicio == p2.inicio) && (p1.final == p2.final)){
        return true;
    }
    else{
        if(anterior1(p1.inicio,p2.inicio)){
            if(anterior1(p1.final,p2.inicio)){
                return false;
            }
            if(anterior1(p2.inicio,p1.final)){
                return true;
            }
        }
        else{
            if(anterior1(p2.inicio,p1.inicio)){
                if(anterior1(p2.final,p1.inicio)){
                    return false;
                }
                if(anterior1(p1.inicio,p2.final)){
                    return true;
                }
            }
        }
    }
}