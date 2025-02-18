//-------------------------------------------------------------------------------------------
// File:   instante.cpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------
#include "instante.hpp"

//Pre: ...
//Post: Devuelve true si los datos introducidos tienen un formato correcto (0<mes<12, dia<31, anyo>=1582, 0<=minuto<60)
//y crea un dato i de tipo instante a partir de ellos. Si no son válidos, devuelve false.
bool crearInstante(const int dia, const int mes, const int anyo, const int hora, const int minuto, instante &i){
    if(mes > 12 || mes <= 0) {
        return false;
    }
    else if(dia <=0 || dia > 31){
        return false;
    }
    else if(hora <0 || hora >= 24){
        return false;
    }
    else if(minuto <0 || minuto >= 60){
        return false;
    }
    else{
            if(((anyo%4 == 0) && (anyo%100 !=0)) || (anyo%400 ==0)){
                if(mes==2){
                    if(dia<=29){
                        i.minuto = minuto;
                        i.hora = hora;
                        i.dia = dia;
                        i.mes = mes;
                        i.anyo = anyo;
                        return true;
                    }
                    else{
                        return false;
                    }
                }
                else if(mes==4 || mes==6 || mes==9 || mes==11){
                    if(0<=dia<=30){
                        i.minuto = minuto;
                        i.hora = hora;
                        i.dia = dia;
                        i.mes = mes;
                        i.anyo = anyo;
                        return true;
                    }
                    else{
                        return false;
                    }
                }
                else{
                    if(0<=dia<=31){
                        i.minuto = minuto;
                        i.hora = hora;
                        i.dia = dia;
                        i.mes = mes;
                        i.anyo = anyo;
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
            else{
                if(mes==2){
                    if(dia<=28){
                        i.minuto = minuto;
                        i.hora = hora;
                        i.dia = dia;
                        i.mes = mes;
                        i.anyo = anyo;
                        return true;
                    }
                    else{
                        return false;
                    }
                }
                else if(mes==4 || mes==6 || mes==9 || mes==11){
                    if(dia<31){
                        i.minuto = minuto;
                        i.hora = hora;
                        i.dia = dia;
                        i.mes = mes;
                        i.anyo = anyo;
                        return true;
                    }
                    else{
                        return false;
                    }
                }
                else{
                        i.minuto = minuto;
                        i.hora = hora;
                        i.dia = dia;
                        i.mes = mes;
                        i.anyo = anyo;
                        return true;
                }
            }
        }
        return false;
    }

//Pre: Pasas por referencia un dato de tipo instante válido
//Post: devuelve el entero dia del instante i
int dia(const instante &i){
    return i.dia;
}

//Pre: Pasas por referencia un dato de tipo instante válido
//Post: devuelve el entero mes del instante i
int mes(const instante &i){
    return i.mes;
}

//Pre: Pasas por referencia un dato de tipo instante válido
//Post: devuelve el entero anyo del instante i
int anyo(const instante &i){
    return i.anyo;
}

//Pre: Pasas por referencia un dato de tipo instante válido
//Post: devuelve el entero hora del instante i
int hora(const instante &i){
    return i.hora;
}

//Pre: Pasas por referencia un dato de tipo instante válido
//Post: devuelve el entero minuto del instante i
int minuto(const instante &i){
    return i.minuto;
}

//Pre: Pasas por referencia dos datos de tipo instante válidos
//Post: devuelve true si los instantes son iguales, y false en caso contrario.
bool operator==(const instante &i1, const  instante &i2){
    return (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)==dia(i2) && hora(i1)==hora(i2) && minuto(i1)==minuto(i2));
}

//Pre: Pasas por referencia dos datos de tipo instante válidos
//Post: devuelve true si el instante i1 es anterior al i2, y false en caso contrario.
bool anterior1(const instante &i1, const  instante &i2){
    return ((anyo(i1)<anyo(i2)) || (anyo(i1)==anyo(i2) && mes(i1)<mes(i2)) || (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)<dia(i2)) || (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)==dia(i2) && hora(i1)<hora(i2)) || (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)==dia(i2) && hora(i1)==hora(i2) && minuto(i1)<minuto(i2)));
}

//Pre: Pasas por referencia dos datos de tipo instante válidos
//Post: devuelve true si el instante i1 es posterior al i2, y false en caso contrario.
bool posterior1(const instante &i1, const  instante &i2){
    return ((anyo(i1)>anyo(i2)) || (anyo(i1)==anyo(i2) && mes(i1)>mes(i2)) || (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)>dia(i2)) || (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)==dia(i2) && hora(i1)>hora(i2)) || (anyo(i1)==anyo(i2) && mes(i1)==mes(i2) && dia(i1)==dia(i2) && hora(i1)==hora(i2) && minuto(i1)>minuto(i2)));
}

//Pre: Pasas por referencia dos datos de tipo instante válidos
//Post:devuelve un entero con el número total de minutos comprendidos
// desde el instante i1 hasta el instante i2. Si ambos instantes 
// i1 e i2 no pertenecen al mismo año y mes, entonces devolverá el entero de valor absoluto 100.0001. 
// El resultado será un número positivo si anterior(i1,i2) o será negativo si posterior(i1,i2).
int diferencia(const instante &i1, const  instante &i2){
    int suma;
    if (i1.mes==i2.mes && i1.anyo==i2.anyo)
    {
        if(i1 == i2){
            return 0;
        }
        if(anterior1(i1,i2)){
            return (((i2.dia-i1.dia)*24*60)+ ((i2.hora-i1.hora)*60) + (i2.minuto-i1.minuto));
        }
        if(posterior1(i1,i2)){
            return -(((i1.dia-i2.dia)*24*60)+ ((i1.hora-i2.hora)*60) + (i1.minuto-i2.minuto));
        }
    }
    else{
        return 100000;
    }
}

