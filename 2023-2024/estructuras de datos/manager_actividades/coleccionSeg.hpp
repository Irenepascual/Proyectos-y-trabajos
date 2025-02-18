//-------------------------------------------------------------------------------------------
// File:   coleccionSeg.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------

#ifndef COLECCIONSEG_HPP
#define COLECCIONSEG_HPP

#include <iostream>
using namespace std;

template<typename segm, typename val> struct coleccionSeg;

template<typename segm, typename val> void crear(coleccionSeg<segm,val>& c);
template<typename segm, typename val> int tamagno(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool esVacia(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool agnadir(coleccionSeg<segm,val>& c, const segm& s, const val& v);
template<typename segm, typename val> bool actualizar(coleccionSeg<segm,val>& c, const segm& s, const val& v);
template<typename segm, typename val> bool esta(coleccionSeg<segm,val>& c, const segm& s);
template<typename segm, typename val> bool obtener(coleccionSeg<segm,val>& c, const segm& s, val& v);
template<typename segm, typename val> bool seSolapa(coleccionSeg<segm,val>& c, const segm& s);
template<typename segm, typename val> bool borrar(coleccionSeg<segm,val>& c, const segm& s);
template<typename segm, typename val> void iniciarIterador(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool haySiguiente(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool siguienteSegm(coleccionSeg<segm,val>& c, segm& s);
template<typename segm, typename val> bool siguienteVal(coleccionSeg<segm,val>& c, val& v);
template<typename segm, typename val> bool avanza(coleccionSeg<segm,val> &c);

template<typename segm, typename val> struct coleccionSeg {
    friend void crear<segm,val>(coleccionSeg<segm,val>& c);
    friend int tamagno<segm,val>(coleccionSeg<segm,val>& c);
    friend bool esVacia<segm,val>(coleccionSeg<segm,val>& c);
    friend bool agnadir<segm,val>(coleccionSeg<segm,val>& c, const segm& s, const val& v);
    friend bool actualizar<segm,val>(coleccionSeg<segm,val>& c, const segm& s, const val& v);
    friend bool esta<segm,val>(coleccionSeg<segm,val>& c, const segm& s);
    friend bool obtener<segm,val>(coleccionSeg<segm,val>& c, const segm& s, val& v);
    friend bool seSolapa<segm,val>(coleccionSeg<segm,val>& c, const segm& s);
    friend bool borrar<segm,val>(coleccionSeg<segm,val>& c, const segm& s);
    friend void iniciarIterador<segm,val>(coleccionSeg<segm,val>& c);
    friend bool haySiguiente<segm,val>(coleccionSeg<segm,val>& c);
    friend bool siguienteSegm<segm,val>(coleccionSeg<segm,val>& c, segm& s);
    friend bool siguienteVal<segm,val>(coleccionSeg<segm,val>& c, val& v);
    friend bool avanza<segm,val>(coleccionSeg<segm,val> &c);

    private:
        struct nodo{
            segm segmento;
            val valor;
            nodo* sig;
        };

        int tamagno;
        nodo* ultimo;
        nodo* primero;
        nodo* iter;
};

//Pre: Los valores del TAD representan colecciones de elementos formados como parejas 
// (segm, val) tales que, en la colección no se permiten elementos con segm iguales o solapados.
//Post: Crea una colección vacía, sin elementos.
template<typename segm, typename val>
void crear(coleccionSeg<segm,val>& c){
    c.tamagno = 0;
    c.primero = nullptr;
}

//Pre: Se le pasa una colección c
//Post: devuelve el tamanyo de la coleccion.
template<typename segm, typename val>
int tamagno(coleccionSeg<segm,val>& c){
    return c.tamagno;
}

//Pre: Recibe ua colección c válida
//Post: Devuelve true si está vacía (tamagno=0), y false en caso contrario.
template<typename segm, typename val>
bool esVacia(coleccionSeg<segm,val>& c){
    return (c.tamagno == 0);
}

//Pre: Recibe una colección válida, y un segmento y un valor determinado.
//Post: Si en c hay algún elemento con segm igual a s, devuelve una colección igual a la resultante de 
//      eliminar ese elemento de la colección c devolviendo true. En caso contrario, devuelve false y una colección igual a c.}
template<typename segm, typename val>
bool agnadir(coleccionSeg<segm,val>& c, const segm& s, const val& v){
    typename coleccionSeg<segm,val>::nodo* dato = new typename coleccionSeg<segm,val>::nodo;
    dato -> segmento = s;
    dato -> valor = v;
    if(esVacia(c)){
        c.primero = dato;
        c.primero -> sig = nullptr;
        c.tamagno++;
        return true;

    }
    else{
        if(seSolapa(c,s)){
            return false;
        }
        else{
            typename coleccionSeg<segm,val>::nodo* aux2 = c.primero;
            if(anterior(s, c.primero->segmento)){
                c.primero = dato;
                c.primero -> sig = aux2;
                c.tamagno++;
                return true;
            }
            else{
                while(aux2 -> sig != nullptr && (!anterior(s, aux2->sig->segmento))){
                    aux2 = aux2 -> sig;
                }
                if(aux2 -> sig == nullptr){
                    aux2 -> sig = dato;
                    dato -> sig = nullptr;
                    c.tamagno++;
                    return true;
                }
                else{
                    typename coleccionSeg<segm,val>::nodo* aux3 = aux2->sig;
                    aux2 -> sig = dato;
                    dato -> sig = aux3;
                    c.tamagno++;
                    return true;
                }
            }
        }
    }
}

//Pre: Recibe una colección válida, y un segmento y un valor determinado.
//Post: Si en c hay algún elemento con segm igual a s, sea (s, v’), devuelve true y una colección igual a la 
//      resultante de sustituir dicho elemento por (s, v) en la colección c. En cambio, devuelve
//      false si en c no hay ningún elemento con segm igual a s.
template<typename segm, typename val>
bool actualizar(coleccionSeg<segm,val>& c, const segm& s, const val& v){
    if (!esta(c,s)){
        return false;
    }
    else{

        typename coleccionSeg<segm,val>::nodo* aux1 = c.primero;

        if(iguales(aux1->segmento, s)){
            aux1 -> valor = v;
            return true;
        }
        else{
            while(aux1 -> sig != nullptr){
                if(!iguales(aux1->sig->segmento,s)){
                    aux1 = aux1 -> sig;
                }
                else{
                    aux1->sig->valor = v;
                    return true;
                }
            }
            
        }
    }
}

//Pre: Recibe una coleccion c y un segmento s válido.
//Post: Devuelve verdad si y sólo si en c hay algún elemento con segm igual a s.
template<typename segm, typename val>
bool esta(coleccionSeg<segm,val>& c, const segm& s){
    if(esVacia(c)){
        return false;
    }    
    else{
        typename coleccionSeg<segm,val>::nodo* aux1 = c.primero;
        if(iguales(aux1->segmento,s)){
            return true;
        }
        else{
            while(aux1 -> sig != nullptr){
                if(!iguales(aux1->sig->segmento, s)){
                    aux1 = aux1 -> sig;
                }
                else{
                    return true;
                }
            }
            return false;
        }
    }
}

//Pre:  Recibe una coleccion c, un segmento s y un valor v, todos ellos con un formato válido.
//Post: Devuelve true si en c hay algún elemento con segm igual a s, y además devuelve el dato val que forma el elemento con dicho s. 
//      En cambio, si en c no hay ningún elemento con segm igual a s devuelve false.
template<typename segm, typename val>
bool obtener(coleccionSeg<segm,val>& c, const segm& s, val& v){
    if(esta(c,s) || !esVacia(c)){
        typename coleccionSeg<segm,val>::nodo* aux1 = c.primero;        
        if(iguales(aux1-> segmento,s)){
            v = aux1 -> valor;
        }
        else{
            while(aux1 -> sig != nullptr){
                if(!iguales(aux1->sig->segmento,s)){
                    aux1 = aux1 -> sig;
                }
                else{
                    v = aux1->sig->valor;            
                    return true;
                }
            }
            return false;
        }
    } 
    return false; 
}

//Pre:  Recibe una colección c, y un segmento s
//Post: Devuelve verdad si y sólo si en c hay algún elemento con un segm que se solape con s
template<typename segm, typename val>
bool seSolapa(coleccionSeg<segm,val>& c, const segm& s){
    if(esVacia(c)){
        return false;
    }
    else{
        typename coleccionSeg<segm,val>::nodo* aux1 = c.primero;
        if(solapados(aux1-> segmento,s)){
            return true;
        }
        else{
            while(aux1 -> sig != nullptr){
                if(solapados(s, aux1->sig->segmento)){
                    return true;
                }
                else{
                    aux1 = aux1 -> sig;
                }
            }
            return false;
        }
    }
}


//Pre: Recibe una colección c, y un segmento s
//Post: Si en c hay algún elemento con segm igual a s, devuelve una colección igual a la resultante de 
//      eliminar ese elemento de la colección c. En caso contrario, devuelve una colección igual a c.
template<typename segm, typename val>
bool borrar(coleccionSeg<segm,val>& c, const segm& s){
    if(esVacia(c)){
        return false;
    }
    else{
        typename coleccionSeg<segm,val>::nodo* aux1 = c.primero;
        typename coleccionSeg<segm,val>::nodo* aux2 = aux1 -> sig;


        if(iguales(aux1-> segmento,s)){
            c.primero=aux2;
            c.tamagno--;
            return true;
        }
        else{
            while(aux1 -> sig != nullptr){
                if(iguales(aux2 -> segmento,s)){
                    if(aux2->sig == nullptr){
                        c.tamagno--;
                        aux1-> sig =nullptr;
                        return true;
                    }
                    else{
                        typename coleccionSeg<segm,val>::nodo* aux3 = aux2 -> sig;
                        aux1->sig = aux2->sig;
                        delete aux2;
                        c.tamagno--;
                        return true;
                    }
                }
                else{
                    aux1 = aux1 -> sig;
                    aux2 = aux1 -> sig;
                }
            }
            return false;
        }
    }
}


//Pre: Recibe una coleccion c.
//Post: Inicializa el iterador de forma dinámica para recorrer los elementos de la colección c, de forma que el siguiente 
//      elemento a visitar sea el que tiene un segm anterior a los de todos los demás elementos de la 
//      colección (situación de no haber visitado ningún elemento)
template<typename segm, typename val>
void iniciarIterador(coleccionSeg<segm,val>& c){
    c.iter=c.primero;
}

//Pre: Recibe una colección c.
//Post: Devuelve true si y solo si queda algún elemento por visitar con el iterador de la colección c, mientras que
//      si ya no quedan elementos por visitar, devuelve false
template<typename segm, typename val>
bool haySiguiente(coleccionSeg<segm,val>& c){
    return (c.iter!=nullptr);
}

//Pre: recibe una coleccion c y un segmento s
//Post: Devuelve true si quedan elementos por visitar y devuelve el siguiente segmento a visitar. Si ya no quedan segmentos
//      por visitar, devuelve false
template<typename segm, typename val>
bool siguienteSegm(coleccionSeg<segm,val>& c, segm& s){
    if (!haySiguiente(c))
    {
        return false;
    }
    else{
        s = c.iter->segmento;
        return true;
    }
    
}

//Pre: recibe un dato c de tipo coleccionSeg, y un valor v.
//Post: Devuelve true si quedan elementos por visitar y devuelve el siguiente valor a visitar. Si ya no quedan valores por
//      visitar, devuelve false.
template<typename segm, typename val>
bool siguienteVal(coleccionSeg<segm,val>& c, val& v){
    if (!haySiguiente(c))
    {
        return false;
    }
    else{
        v = c.iter->valor;
        return true;
    }
    
}

//Pre: recibe una coleccion c
//Post: Si quedan elementos por visitar en la coleccion,
//      avanza el iterador para visitar el siguiente elemento de la coleccion y devuelve true, mientras
//      que si no quedan devuelve false.
template<typename segm, typename val>
bool avanza(coleccionSeg<segm,val>& c){
    if (!haySiguiente(c))
    {
        return false;
    }
    else{
        c.iter=c.iter->sig;
        return true;
    }
}

#endif
