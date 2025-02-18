//-------------------------------------------------------------------------------------------
// File:   pila.hpp
// Author: profesores de la asignatura
//-------------------------------------------------------------------------------------------
#ifndef PILA_DINAMICA_HPP
#define PILA_DINAMICA_HPP
   
using namespace std;


template<typename T> struct Pila;
template<typename T> void crearVacia(Pila<T>& p);
template<typename T> void apilar(Pila<T>& p, const T& e);
template<typename T> bool desapilar(Pila<T>& p);
template<typename T> bool cima(const Pila<T>& p, T& e);
template<typename T> bool esVacia(const Pila<T>& p);
template<typename T> int altura(const Pila<T>& p);
template<typename T> void duplicar(const Pila<T>& pilaEnt, Pila<T>& pilaSal);
template<typename T> bool operator==(const Pila<T>& p1, const Pila<T>& p2);
template<typename T> void liberar(Pila<T>& p);
template<typename T> void iniciarIteradorP(Pila<T>& p);
template<typename T> bool existeSiguiente(const Pila<T>& p);
template<typename T> bool siguiente(Pila<T>& p, T& e);


template<typename T>
struct Pila {
    friend void crearVacia<T>(Pila<T>& p);
    friend void apilar<T>(Pila<T>& p, const T& e);
    friend bool desapilar<T>(Pila<T>& p);
    friend bool cima<T>(const Pila<T>& p, T& e);
    friend bool esVacia<T>(const Pila<T>& p);
    friend int altura<T>(const Pila<T>& p);
    friend void duplicar<T>(const Pila<T>& pilaEnt, Pila<T>& pilaSal);
    friend bool operator==<T>(const Pila<T>& p1, const Pila<T>& p2);
    friend void liberar<T>(Pila<T>& p);
    friend void iniciarIteradorP<T>(Pila<T>& p);
    friend bool existeSiguiente<T>(const Pila<T>& p);
    friend bool siguiente<T>(Pila<T>& p, T& e);
 private:
  	struct Nodo { // tipo de dato para cada nodo de la lista enlazada
 	  	T dato;  
		  Nodo* sig;			
    };

	  Nodo* cim; // cima de la pila		
	  int alt;   // altura de la pila
  	Nodo* itr; // para implementar el iterador
};



/* Devuelve en p la Pila vac�a, sin elementos.
*/
template<typename T> 
void crearVacia(Pila<T>& p) {
    p.cim = nullptr;
    p.alt = 0;
}



/* A�ade e en p como nueva cima de la Pila p. 
*/
template<typename T> 
void apilar(Pila<T>& p, const T& e) {
    typename Pila<T>::Nodo* aux = p.cim; //guardar puntero a la anterior cima
    p.cim = new typename Pila<T>::Nodo;  // reservar memoria para el Nodo que contendr� la nueva cima
    //asignar el elemento de la nueva cima, encadenarla con la anterior cima, e incrementar en 1 la altura:
    p.cim->dato = e;
    p.cim->sig = aux;
    p.alt = p.alt + 1;
}


/* Si p es no vac�a, devuelve p tras eliminar de ella el elemento que era cima de p (el �ltimo 
   elemento que fue apilado). Si p es vac�a, la deja igual. 
*/
template<typename T> 
bool desapilar(Pila<T>& p) {
    if (!esVacia(p)) { // si la pila no es vac�a
        typename Pila<T>::Nodo* aux = p.cim; //puntero al Nodo con la cima a eliminar
        p.cim = p.cim->sig; // actualizar cim a la anterior cima de la pila
        delete aux;         //liberar la memoria ocupada por el Nodo a eliminar
		p.alt = p.alt - 1;  //decrementar la altura de la pila en 1
		return true;
    }
	return false;
}


/* Si p es no vac�a, devuelve en e el �ltimo elemento apilado en p (es decir, su cima), y devuelve error=false.
   Si p es vac�a, devuelve error=true. 
*/
template<typename T> 
bool cima(const Pila<T>& p, T& e) {
    if (esVacia(p)) {
		return false;
    } else {
		e = p.cim->dato;
		return true;
    }
}


/* Devuelve true si y s�lo si p no tiene elementos, es decir la pila est� vac�a y p.cim es nullptr.
*/
template<typename T> 
bool esVacia(const Pila<T>& p) {
    return p.cim == nullptr;
}

/* Devuelve el n�mero de elementos que hay en p. 
*/
template<typename T> 
int altura(const Pila<T>& p) {
    return p.alt;
}

/* Devuelve en pSal una copia profunda de la Pila pEnt, es decir duplicando la representaci�n en memoria. 
Cada elemento de la pila ser� copiado utilizando el operador de copia = de los datos de tipo T.
*/
template<typename T> 
void duplicar(const Pila<T>& pilaEnt, Pila<T>& pilaSal){
	if (esVacia(pilaEnt)) {
		crearVacia(pilaSal);
	} else {
		typename Pila<T>::Nodo* ptEnt = pilaEnt.cim;  //puntero ptEnt apuntar� en todo momento al Nodo de pilaEnt que se va a copiar 
		pilaSal.cim = new typename Pila<T>::Nodo;
		pilaSal.cim->dato = ptEnt->dato;              // se copia el elemento cima de la pilaEnt como cima de pilaSal
		typename Pila<T>::Nodo* ptSal = pilaSal.cim;  // puntero ptSal apuntar� en todo momento al �ltimo Nodo de la lista enlazada que se est� construyendo para pilaSal
		ptEnt = ptEnt->sig;
		while (ptEnt != nullptr) {    //Mientras no se hayan copiado los elementos de todos los Nodos de la lista de pilaEnt
			ptSal->sig = new typename Pila<T>::Nodo;   // crear y encadenar un nuevo nodo al final de la lista enlazada en construcci�n para pilaSal
			ptSal = ptSal->sig;
			ptSal->dato = ptEnt->dato;                 //copiar el dato para ese Nodo
			ptEnt = ptEnt->sig;
		}
		//Se marca el final de la lista enlazada construida para pilaSal, y se copia la altura de las pilas:
		ptSal->sig = nullptr;
		pilaSal.alt = pilaEnt.alt;
	}
}


/* Devuelve true si y s�lo si p1 y p2 tienen los mismos elementos y en las mismas posiciones. 
Cada elemento de la pila ser� comparado utilizando el operador == de los datos de tipo T.
*/
template<typename T> 
bool operator==(const Pila<T>& p1, const Pila<T>& p2){
	if (p1.alt != p2.alt) { 
		return false;
	} else {    // Ambas pilas tienen el mismo n�mero de elementos
		typename Pila<T>::Nodo* pt1 = p1.cim;  
		typename Pila<T>::Nodo* pt2 = p2.cim;   //pt1 y pt2 se utilizan para recorrer las listas enlazadas y comprobar elementos en la misma posici�n de p1 y p2 respectivamente
		bool iguales = true;
		while (pt1 != nullptr && iguales) {     //mientras no se haya encontrado una diferencia y a�n queden elementos por comparar en las listas...
			iguales = (pt1->dato == pt2->dato); //comparamos los 2 elementos en la misma posici�n actual en ambas listas
			pt1 = pt1->sig;                    // se avanza la posici�n en ambas listas a la vez, por tanto se acabar�n ambas a la vez (tienen el mismo n�mero de elementos)
			pt2 = pt2->sig;
		}
		return iguales;
	}
}


/* Libera toda la memoria din�mica utilizada por p y devuelve en p la Pila vac�a. 
*/
template<typename T> 
void liberar(Pila<T>& p) {
    typename Pila<T>::Nodo* aux = p.cim;
    while (aux != nullptr) {
        p.cim = p.cim->sig;
        delete aux;
        aux= p.cim;
    }
	p.alt = 0;
}


/* Las tres operaciones siguientes conforman un iterador interno para la Pila: 
Este iterador permitir� visitar los elementos almacenados en la pila empezando por el elemento que est� en la cima y acabando con el primer elemento que se introdujo en la pila.
Si el puntero itr de la pila tiene valor nullptr entonces no existe elemento pendiente por visitar (ya se han visitado todos los elementos), 
en caso contrario el puntero apunta al Nodo cuyo dato es el siguiente elemento a visitar.
*/

/* Prepara el iterador para que el siguiente elemento a visitar sea un primer 
   elemento de p (es decir, el que est� en la cima de p), si existe (situaci�n de no haber visitado ning�n elemento). 
*/
template<typename T> 
void iniciarIteradorP(Pila<T>& p) {
	p.itr = p.cim;
}


/* Devuelve false si ya se han visitado todos los elementos de p; devuelve true en caso contrario. 
*/
template<typename T> 
bool existeSiguiente(const Pila<T>& p) {
	return p.itr != nullptr;
}

/* Si existe alg�n elemento de p pendiente de visitar, devuelve en e el siguiente 
   elemento a visitar y error=false, y adem�s avanza el iterador para que a 
   continuaci�n se pueda visitar otro elemento de p. Si no quedan elementos pendientes 
   de visitar devuelve error=true 
*/
template<typename T> 
bool siguiente(Pila<T>& p, T& e) {
	if (existeSiguiente(p)) {
		e = p.itr->dato;     // 1o) devolver el elemento que toca visitar
		p.itr = p.itr->sig;  // 2o) avanzar el iterador para que est� disponible el siguiente elemento 
		return true;
	} else {
		return false;
	}
}

#endif //PILA_DINAMICA_HPP