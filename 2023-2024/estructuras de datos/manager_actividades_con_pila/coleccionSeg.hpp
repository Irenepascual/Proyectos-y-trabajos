//-------------------------------------------------------------------------------------------
// File:   coleccionSeg.hpp
// Author: Irene Pascual Albericio ; Sergio Ros Alcázar 
//-------------------------------------------------------------------------------------------

#ifndef COLECCIONSEG_HPP
#define COLECCIONSEG_HPP

#include "pila.hpp"
#include <iostream>
using namespace std;

template<typename segm, typename val> struct coleccionSeg;

template<typename segm, typename val> void crear(coleccionSeg<segm,val>& c);
template<typename segm, typename val> int tamagno(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool esVacia(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool agnadir_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, const val& v);
template<typename segm, typename val> bool agnadir(coleccionSeg<segm,val>& c, const segm& s, const val& v);
template<typename segm, typename val> bool actualizar_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, const val& v);
template<typename segm, typename val> bool actualizar(coleccionSeg<segm,val>& c, const segm& s, const val& v);
template<typename segm, typename val> bool esta_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s);
template<typename segm, typename val> bool esta(coleccionSeg<segm,val>& c, const segm& s);
template<typename segm, typename val> bool obtener_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, val& v);
template<typename segm, typename val> bool obtener(coleccionSeg<segm,val>& c, const segm& s, val& v);
template<typename segm, typename val> bool seSolapa_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s);
template<typename segm, typename val> bool seSolapa(coleccionSeg<segm,val>& c, const segm& s);
template<typename segm, typename val> bool borrarMax(typename coleccionSeg<segm,val>::nodo*& a, segm& s, val& v);
template<typename segm, typename val> bool borrar_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s);
template<typename segm, typename val> bool borrar(coleccionSeg<segm,val>& c, const segm& s);
template<typename segm, typename val> void iniciarIterador(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool haySiguiente(coleccionSeg<segm,val>& c);
template<typename segm, typename val> bool siguienteSegmVal(coleccionSeg<segm,val>& c, segm& s, val& v);
template<typename segm, typename val> struct coleccionSeg {
        private:
        struct nodo{
            segm segmento;
            val valor;
            nodo* izq;
            nodo* der;
        };

        int numElems;
        nodo* raiz;
        Pila<nodo* > iter;

    public:
    friend void crear<segm,val>(coleccionSeg<segm,val>& c);
    friend int tamagno<segm,val>(coleccionSeg<segm,val>& c);
    friend bool esVacia<segm,val>(coleccionSeg<segm,val>& c);
    friend bool agnadir_rec<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, const val& v);
    friend bool agnadir<segm,val>(coleccionSeg<segm,val>& c, const segm& s, const val& v);
    friend bool actualizar_rec<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, const val& v);
    friend bool actualizar<segm,val>(coleccionSeg<segm,val>& c, const segm& s, const val& v);
    friend bool esta_rec<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, const segm& s);
    friend bool esta<segm,val>(coleccionSeg<segm,val>& c, const segm& s);
    friend bool obtener_rec<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, val& v);
    friend bool obtener<segm,val>(coleccionSeg<segm,val>& c, const segm& s, val& v);
    friend bool seSolapa_rec<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, const segm& s);
    friend bool seSolapa<segm,val>(coleccionSeg<segm,val>& c, const segm& s);
    friend bool borrarMax<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, segm& s, val& v);
    friend bool borrar_rec<segm,val>(typename coleccionSeg<segm,val>::nodo*& a, const segm& s);
    friend bool borrar<segm,val>(coleccionSeg<segm,val>& c, const segm& s);
    friend void iniciarIterador<segm,val>(coleccionSeg<segm,val>& c);
    friend bool haySiguiente<segm,val>(coleccionSeg<segm,val>& c);
    friend bool siguienteSegmVal<segm,val>(coleccionSeg<segm,val>& c, segm& s, val& v);
      
};

//Pre: Los valores del TAD representan colecciones de elementos formados como parejas 
// (segm, val) tales que, en la colección no se permiten elementos con segm iguales o solapados.
//Post: Crea una colección vacía, sin elementos.
template<typename segm, typename val>
void crear(coleccionSeg<segm,val>& c){
    c.numElems = 0;
    c.raiz = nullptr;
}

//Pre: Se le pasa una colección c
//Post: devuelve el tamanyo de la coleccion.
template<typename segm, typename val>
int tamagno(coleccionSeg<segm,val>& c){
    return c.numElems;
}

//Pre: Recibe ua colección c válida
//Post: Devuelve true si está vacía (tamagno=0), y false en caso contrario.
template<typename segm, typename val>
bool esVacia(coleccionSeg<segm,val>& c){
    return (c.numElems == 0);
}

//Pre:El segmento a añadir no se solapa o no es igual a ningún segmento de la colección c.
//Post:la función recorre el arbol de forma recursiva y ordenada, y una vez encuentra la rama en la que añadir el elemento, devuelve true
//     y lo añade. Si no se ha añadido correctamente, devuelve false.
template<typename segm, typename val>
bool agnadir_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, const val& v){
        if(a == nullptr){   //ha llegado al lugar donde debe ser añadido
            a = new typename coleccionSeg<segm,val>::nodo; //creamos nuevo nodo para añadir el nuevo dato
            a->segmento = s;
            a->valor = v;
            a-> izq = nullptr;
            a-> der = nullptr;
            return true;
        }
        else{
            if(anterior(s,a->segmento)){ //si el segmento que queremos añadir es menor al que estamos en ese momento, continuamos por la izquierda.
                return agnadir_rec(a->izq, s, v);
            }
            
            else if((!anterior(s,a->segmento))){ //si el segmento que queremos añadir es mayor al que estamos en ese momento, continuamos por la derecha.
                return agnadir_rec(a->der, s, v);
            }
            else{
                return false;
            }
        }
}

//Pre: Recibe una colección válida, y un segmento y un valor determinado.
//Post:Si en c no hay ningún elemento con segm igual o solapado con s, devuelve una colección igual a
//     la resultante de añadir el elemento (s, v) a la colección c, aumentando el número de elementos
//     de la colección. En caso contrario, devuelve una colección igual a c.
template<typename segm, typename val>
bool agnadir(coleccionSeg<segm,val>& c, const segm& s, const val& v){
    if(!seSolapa(c,s)){ //comprueba si se solapa con alguno, y si no solapa, pasa a añadirlo
        if(agnadir_rec(c.raiz,s,v)){
            c.numElems++;   //como lo ha añadido, aumentan el numero de elementos.
            return true;
        }
    }
    else{
        return false;
    }
}

//Pre: la colección c no es vacía
//Post: la funcion recorre el árbol de forma recursiva y ordenada, y si encuentra un elemento con segmento = s,
//      actualiza el valor asociado a ese segmento en la coleccion c, mientras que si no encuentra el mismo segmento,
//      devuelve false.
template<typename segm, typename val>
bool actualizar_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, const val& v){
    if(a==nullptr){ //si hemos llegado al final y no lo hemos conseguido actualizar, devuelve false.
        return false;
    }
    else{
    if(iguales(s,a->segmento)){ //Cuando encuentra el segmento que coincide con el segmento que buscabamos, cambiamos el valor. 
        a->valor = v;
        return true;
    }
    else{
        if(anterior(s, a->segmento)){ //si el segmento que estamos buscando es menor al que estamos en ese momento, seguimos por la izquierda.
            return actualizar_rec(a->izq,s,v); 
        }
        if(anterior(a->segmento, s)){ //si el segmento que estamos buscando es mayor al que estamos en ese momento, seguimos por la derecha.
            return actualizar_rec(a->der,s,v);
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
    if(!esVacia(c)){ //si no es vacia, podra actualizar el valor
        return actualizar_rec(c.raiz,s,v);
    }
    else{
        return false;
    }
}

//Pre: la colección c no es vacía
//Post: Recorre el arbol(c) de forma recursiva y ordenada, y devuelve true si encuentra a lo largo de la
//      búsqueda un segmento igual a s. Si recorre todo el arbol y no lo encuentra, devuelve false.
template<typename segm, typename val>
bool esta_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s){
    if(a == nullptr){ //si llega al final y no lo ha encontrado, es que no está.
        return false;
    }
    else{
        if(anterior(s,a->segmento)){  //si el segmento que estamos buscando es menor al que estamos en ese momento, seguimos por la izquierda.
            return esta_rec<segm,val>(a->izq, s); 
        }
        if (iguales(s, a-> segmento)){ //si es el segmento que estamos buscando, lo hemos encontrado y devuelve true.
            return true;
        }
        if(!(anterior(s,a->segmento))){ //si el segmento que estamos buscando es mayor al que estamos en ese momento, seguimos por la derecha.
                return esta_rec<segm,val>(a->der, s); 
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
        return esta_rec<segm,val>(c.raiz,s); //si no es vacía, busca a ver si está el segmento que nos dan.
    }
}

template<typename segm, typename val>
bool obtener_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s, val& v){
    if(a==nullptr){ //si hemos llegado al final, es que no está el segmento que buscamos.
        return false;
    }
    else{
        if(iguales(s,a->segmento)){ //si lo hemos encontrado, devuelve el valor de v y true.
            v = a->valor;
            return true;
        }
        else{
            if(anterior(s, a->segmento)){ //si el segmento que estamos buscando es menor al que estamos en ese momento, seguimos por la izquierda.
                return obtener_rec(a->izq, s, v); 
            }
            if(anterior(a->segmento, s)){ //si el segmento que estamos buscando es mayor al que estamos en ese momento, seguimos por la izquierda.
                return obtener_rec(a->der, s, v); 
            }
        }
    }
}


//Pre:  Recibe una coleccion c, un segmento s y un valor v, todos ellos con un formato válido.
//Post: Devuelve true si en c hay algún elemento con segm igual a s, y además devuelve el dato val que forma el elemento con dicho s. 
//      En cambio, si en c no hay ningún elemento con segm igual a s devuelve false.
template<typename segm, typename val>
bool obtener(coleccionSeg<segm,val>& c, const segm& s, val& v){
    if(!esVacia(c)){ //si no es vacia pasamos a obtener el valor.
        return obtener_rec<segm,val>(c.raiz, s, v);
    }
    else{
        return false;
    }

}

//Pre:  la colección c no es vacía y el segmento s no está dentro de la colección c.
//Post: Recorre el árbol de forma recursiva y ordenada, y en cada rama va comprobando
//      si el segmento asociado a ella está solapado con el segmento s. En caso de que así sea,
//      devuelve verdad, y en caso contrario sigue recorriendo el árbol hasta que o bien se encuentra
//      algún segmento de la coleccion con el que s solapa,o bien acabamos de recorrer el árbol, por lo tanto
//      ningún segmento solapa y devuelve false.
template<typename segm, typename val>
bool seSolapa_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s){
    if(a == nullptr){ //si llega al final y no lo ha encontrado, es que no se solapa
        return false;
    }
    else{
        if(solapados(a-> segmento,s)){  //si se solapan, devuelve true.
            return true;
        }
        else{
            if(anterior(s, a->segmento)){ //si el segmento que estamos buscando es menor al que estamos en ese momento, seguimos por la izquierda.
                return (seSolapa_rec<segm,val>(a->izq, s));
            }
            else if(anterior(a->segmento, s)){ //si el segmento que estamos buscando es mayor al que estamos en ese momento, seguimos por la derecha.
                return (seSolapa_rec<segm,val>(a->der, s));
            }
            else{
                return false;
            }
        }
    }
}

//Pre:  Recibe una colección c, y un segmento s
//Post: Devuelve verdad si y sólo si en c hay algún elemento con un segm que se solape con s
template<typename segm, typename val>
bool seSolapa(coleccionSeg<segm,val>& c, const segm& s){
    if(esta(c,s)){ //si el segmento está tal cual (osea, todo el periodo coincide), se solapa y devuelve true.
        return true;
    }
    else{
        if(!esVacia(c)){ //si no es vacia, pasa a buscar a ver si se solapa
            return seSolapa_rec<segm,val>(c.raiz, s);
        }
        else{
            return false;
        }
    }
}

//Pre: los hijos de la rama a la que apunta el puntero a, son no vacíos
//Post: recorremos la parte derecha del árbol y cuando ya no haya más hijos cambiamos el valor y el segmento de a por el de éste último hijo.
template<typename segm, typename val>
bool borrarMax(typename coleccionSeg<segm,val>::nodo*& a, segm& s, val& v){
    typename coleccionSeg<segm,val>::nodo* aux;
    if(a->der == nullptr){ //si el punterode la derecha es nulo
        s=(a->segmento);
        v=(a->valor);
        aux=a;
        a=a->izq;
        delete(aux);
        return true;
    }
    else{
        return borrarMax<segm,val>(a->der, s, v); //sino, sigue por la derecha
    }
}

//Pre: recibe una coleccion c y un segmento s válidos.
//Post: Recorre el árbol de forma recursiva y ordenada buscando un elemento con segmento igual a s. Si lo encuentra y ambos hijos
//      de la rama son no vacíos, ejecuta la funcion borrarMax, y en el caso de que alguno sea vacío, avanza a esa rama para eliminarlo.
//      Si no lo encuentra devuelve false.
template<typename segm, typename val>
bool borrar_rec(typename coleccionSeg<segm,val>::nodo*& a, const segm& s){
    if (a == nullptr){  //si ha llegado al final, devuelve false porque no lo puede borrar
        return false;
    }
    else{
        if(iguales((a-> segmento), s)){ //si encuentra el elemento que quiere borrar
            if(a->izq == nullptr){ //si el puntero de la izquierda es nulo, entonces pasa el lado de la derecha al lugar donde estamos (el sitio del elemento borrado)
                typename coleccionSeg<segm,val>::nodo* aux=a;
                a=a->der;
                delete(aux);
                return true;
            }
            else{
                return borrarMax<segm,val>(a->izq, a->segmento, a->valor); //sino, llama a la funcion borrarMax por la izquierda.
            }
        }
        if((!anterior(a-> segmento, s))){ //si el segmento que estamos buscando es menor al que estamos en ese momento, seguimos por la izquierda.
            return borrar_rec<segm,val>(a->izq,s);
        }

        if(anterior(a->segmento, s)){ //si el segmento que estamos buscando es mayor al que estamos en ese momento, seguimos por la derecha.
            return borrar_rec<segm,val>(a->der,s); 
        }
    }
}

//Pre: recibe una coleccion c y un segmento s válidos.
//Post: Si en c hay algún elemento con segm igual a s, devuelve una colección igual a la resultante de
//      eliminar ese elemento de la colección c. En caso contrario, devuelve una colección igual a c.
template<typename segm, typename val>
bool borrar(coleccionSeg<segm,val>& c, const segm& s){
   if (borrar_rec<segm,val>(c.raiz, s)){ //si puede borrarlo, devuelve true y decrementa el tamaño de elementos.
        c.numElems--;
        return true;
   }
   else{
    return false;
   }
}


//Pre: Recibe una coleccion c.
//Post: Inicializa el iterador de forma dinámica para recorrer los elementos de la colección c, de forma que el siguiente 
//      elemento a visitar sea el que tiene un segm anterior a los de todos los demás elementos de la 
//      colección (situación de no haber visitado ningún elemento)
template<typename segm, typename val>
void iniciarIterador(coleccionSeg<segm,val>& c){
    typename coleccionSeg<segm,val>::nodo* aux;
    crearVacia(c.iter);
    aux=c.raiz;
    while (aux != nullptr)
    {
        apilar(c.iter,aux);
        aux=aux->izq;
    }
}

//Pre: Recibe una colección c.
//Post: Devuelve true si y solo si queda algún elemento por visitar con el iterador de la colección c, mientras que
//      si ya no quedan elementos por visitar, devuelve false
template<typename segm, typename val>
bool haySiguiente(coleccionSeg<segm,val>& c){
    return (!esVacia(c.iter));
}

//Pre: recibe una coleccion c y un segmento s
//Post: Devuelve true si quedan elementos por visitar y devuelve el siguiente segmento a visitar. Si ya no quedan segmentos
//      por visitar, devuelve false
template<typename segm, typename val>
bool siguienteSegmVal(coleccionSeg<segm,val>& c, segm& s, val& v){
    if (!haySiguiente(c))
    {
        return false;
    }
    else{
        typename coleccionSeg<segm,val>::nodo* aux;
        if(cima(c.iter,aux)){
            s = aux->segmento;
            v = aux->valor;
            desapilar(c.iter);
            aux=aux->der;
            while (aux != nullptr)
            {
                apilar(c.iter, aux);
                aux = aux->izq;
            }
            return true;
        }

    }
    
}

#endif
