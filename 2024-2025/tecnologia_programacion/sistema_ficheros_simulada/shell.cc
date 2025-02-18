#include "shell.h"

/* Devuelve la representación textual de la ruta desde la raiz al directorio actual de trabajo,
   con todos los nombres de los directorios concatenados y separados por '/' */
string Shell::pwd() {
    if ((int)_ruta.size() == 1) {
        return "/";
    }
    else {
        stringstream ss;
        int i=1;
        while (i<(int)_ruta.size()) {
            ss << "/" << _ruta[i]->get_nombre();
            i++;
        }
        return ss.str();
    }
}

/* Devuelve un listado con el nombre de todos los nodos contenidos en el
   directorio actual, uno por línea */
string Shell::ls() {
    stringstream ss;
    if (dynamic_pointer_cast<Directorio>(_ruta.back()) != nullptr) {
        _ruta.back()->mostrar_nombre(ss);
    }
    return ss.str();
}

/* Devuelve un listado con el nombre y el tamaño de todos los nodos contenidos
   en el directorio actual, uno por linea */
string Shell::du() {
    stringstream ss;
    if (dynamic_pointer_cast<Directorio>(_ruta.back()) != nullptr) {
        _ruta.back()->mostrar_nombre_tam(ss);
    }
    return ss.str();
}

/* Edita el fichero de nombre 'name' (en el directorio actual). Para simular la edición, 
   simplemente se cambia el tamaño del fichero al valor especificado como parámetro. 
   Si el fichero no existe, se debe crear con el nombre y tamaño especificados */
void Shell::vi(string name, int size) {
    shared_ptr<Nodo> nodo_encontrado = _ruta.back()->obtener_nodo(name);
    if (nodo_encontrado == nullptr) {
        shared_ptr<Fichero> nuevoFich = make_shared<Fichero>(Fichero(name,size));
        _ruta.back()->insert_elem(nuevoFich);
    }
    else {
        shared_ptr<Fichero> nodo_fichero = dynamic_pointer_cast<Fichero>(nodo_encontrado);
        if (nodo_fichero != nullptr) {
            nodo_fichero->set_tam(size);
        }
        else {
            // Error: ya existe un nodo con ese nombre en el directorio actual que no es un fichero
            throw yaExisteNodoNoFichero();
        }
    }
}

/* Crea un directorio de nombre 'name' en el directorio activo */
void Shell::mkdir(string name) {
    // if (name[name.size()-1] == '/') {
    //     string nuevo_nombre = name;
    //     nuevo_nombre.erase(name.end()-1);
    //     mkdir(nuevo_nombre);
    // }
    // else {
        shared_ptr<Nodo> nodo_existente = _ruta.back()->obtener_nodo(name);
        if (nodo_existente == nullptr) {
            shared_ptr<Directorio> nuevoDir = make_shared<Directorio>(Directorio(name,0));
            _ruta.back()->insert_elem(nuevoDir);
        }
        else {
            // Error: ya existe un nodo con ese nombre en el directorio actual
            throw yaExisteNodo();
        }
    // }  
}

/* Cambia el directorio de trabajo (directorio actual), es decir, hace que la ruta activa cambie
   a otro directorio. La nueva ruta activa definida en 'path' debe determinar un directorio o un
   enlace a un directorio */
/* OBSERVACIÓN: Dada la existencia de enlaces (también entre directorios), la ruta para llegar a
   un nodo determinado puede no ser única (ie, para llegar a un nodo no es obligatorio pasar por
   su padre directo en el árbol de directorios). El camino seguido para llegar a un directorio
   puede incluir enlaces, y no debe cambiar, por ejemplo, el cado de la orden 'cd ..' */
void Shell::cd(string path) {
    if (path != ".") {
        if (path == "..") {
            if (_ruta.size() > 1) {
                _ruta.pop_back();
            }
            else {
                // Error: no existe el directorio padre, ya estamos en el raiz
                throw noHayDirAnterior();
            }
        }
        else if (path == "/") {
            _ruta.erase(_ruta.begin()+1,_ruta.end());
        }
        else {
            string ruta_actual = pwd();
            if (path[0] == '/') {
                _ruta.erase(_ruta.begin()+1,_ruta.end()); // cd("/");
                path.erase(path.begin());
            }
            if ((int)path.find('/') == -1) {
                if (path != ".") {
                    if (path == "..") {
                        if (_ruta.size() > 1) {
                            _ruta.pop_back();
                        }
                        else {
                            cd(ruta_actual);
                            // Error: no existe el directorio padre
                            throw noHayDirAnterior();
                        }
                    }
                    else {
                        shared_ptr<Nodo> nodo_encontrado = _ruta.back()->obtener_nodo(path);
                        if (nodo_encontrado != nullptr) {
                            shared_ptr<Directorio> nodo_directorio = dynamic_pointer_cast<Directorio>(nodo_encontrado);
                            if (nodo_directorio != nullptr) {
                                _ruta.push_back(nodo_directorio);
                            }
                            else {
                                shared_ptr<Enlace> nodo_enlace = dynamic_pointer_cast<Enlace>(nodo_encontrado);
                                if (nodo_enlace != nullptr) {
                                    shared_ptr<Directorio> nodo_apuntado = dynamic_pointer_cast<Directorio>(nodo_enlace->get_link());
                                    if (nodo_apuntado != nullptr) {
                                        bool agnadido = false;
                                        do {
                                            shared_ptr<Nodo> es_ese_nodo = _ruta.back()->obtener_nodo(nodo_apuntado->get_nombre());
                                            if (es_ese_nodo != nullptr) {
                                                _ruta.push_back(nodo_apuntado);
                                                agnadido = true;
                                            }
                                            else {
                                                if (_ruta.size() > 1) {
                                                    cd("..");
                                                }
                                                else {
                                                    cd(ruta_actual);
                                                    throw enlaceInaccesible();
                                                }
                                            }
                                        }
                                        while(!agnadido);
                                    }
                                    else {
                                        cd(ruta_actual);
                                        // Error: el nodo apuntado por el enlace no es un directorio
                                        throw apuntadoNoEsEnlace();
                                    }
                                }
                                else {
                                    cd(ruta_actual);
                                    // Error: el nodo no es un directorio en ese path
                                    throw noEsDirectorio();
                                }
                                
                            }
                        }
                        else {
                            cd(ruta_actual);
                            // Error: no existe un nodo con ese nombre en ese path
                            throw noExisteNodo();
                        }
                    }   
                } 
            }
            else {
                stringstream ss(path);
                string nombre;
                while (getline(ss,nombre,'/')) {
                    // cd(nombre);
                    if (nombre != ".") {
                        if (nombre == "..") {
                            if (_ruta.size() > 1) {
                                _ruta.pop_back();
                            }
                            else {
                                cd(ruta_actual);
                                // Error: no existe el directorio padre, ya estamos en el raiz
                                throw noHayDirAnterior();
                            }
                        }
                        else if (nombre != "") {
                            shared_ptr<Nodo> nodo_encontrado = _ruta.back()->obtener_nodo(nombre);
                            if (nodo_encontrado != nullptr) {
                                shared_ptr<Directorio> nodo_directorio = dynamic_pointer_cast<Directorio>(nodo_encontrado);
                                if (nodo_directorio != nullptr) {
                                    _ruta.push_back(nodo_directorio);
                                }
                                else {
                                    shared_ptr<Enlace> nodo_enlace = dynamic_pointer_cast<Enlace>(nodo_encontrado);
                                    if (nodo_enlace != nullptr) {
                                        shared_ptr<Directorio> nodo_apuntado = dynamic_pointer_cast<Directorio>(nodo_enlace->get_link());
                                        if (nodo_apuntado != nullptr) {
                                            bool agnadido = false;
                                            do {
                                                shared_ptr<Nodo> es_ese_nodo = _ruta.back()->obtener_nodo(nodo_apuntado->get_nombre());
                                                if (es_ese_nodo != nullptr) {
                                                    _ruta.push_back(nodo_apuntado);
                                                    agnadido = true;
                                                }
                                                else {
                                                    if (_ruta.size() > 1) {
                                                        cd("..");
                                                    }
                                                    else {
                                                        cd(ruta_actual);
                                                        throw enlaceInaccesible();
                                                    }
                                                }
                                            }
                                            while(!agnadido);
                                        }
                                        else {
                                            cd(ruta_actual);
                                            // Error: el nodo apuntado por el enlace no es un directorio
                                            throw apuntadoNoEsEnlace();
                                        }
                                    }
                                    else {
                                        cd(ruta_actual);
                                        // Error: el nodo no es un directorio en ese path
                                        throw noEsDirectorio();
                                    }
                                }
                            }
                            else {
                                cd(ruta_actual);
                                // Error: no existe un nodo con ese nombre en ese path
                                throw noExisteNodo();
                            }
                        }
                    }
                }
            }
        }
    }
}


/* Crea en el directorio actual un enlace simbólico de nombre 'name' que apunta
   al elemento identificado mediante la ruta especificada en 'path', que a su vez
   puede ser de cualquier tipo (fichero, directorio u otro enlace). El nombre 'name'
   es un nombre simple de nodo (se creará en el directorio activo), por lo que no
   puede contener una ruta completa. La ruta definida en 'path' sí, de tal modo que
   se pueda crear un enlace a un elemento en otro directorio del árbol, que deberá
   existir previamente */
void Shell::ln(string path, string name) {
    string ruta_actual = pwd();
    int ult_barra = path.find_last_of('/');
    if (ult_barra == (int)path.size()-1) {
        path.erase(path.end()-1);
        ln(path,name);
    }
    else if (ult_barra == -1) {
        // el nodo a enlazar está en el directorio actual
        shared_ptr<Nodo> nodo_a_enlazar = _ruta.back()->obtener_nodo(path);
        if (nodo_a_enlazar != nullptr) {
            shared_ptr<Nodo> nodo_existente = _ruta.back()->obtener_nodo(name);
            if (nodo_existente == nullptr) {
                shared_ptr<Enlace> nuevoEnlace = make_shared<Enlace>(Enlace(name,nodo_a_enlazar));
                _ruta.back()->insert_elem(nuevoEnlace);
            }
            else {
                // Error: ya existe un nodo con ese nombre en ese path
                throw yaExisteNodo();
            }
        }
        else {
            // Error: no existe un nodo con ese nombre en ese path
            throw noExisteNodo();
        }
    }
    else {
        string ruta_nueva = path;
        string nombre_nodo = path;
        ruta_nueva.erase(ruta_nueva.begin()+ult_barra,ruta_nueva.end());
        nombre_nodo.erase(nombre_nodo.begin(),nombre_nodo.begin()+ult_barra+1);
        if (ruta_nueva != "") {
            // el nodo a enlazar esta en ruta_nueva
            cd(ruta_nueva);
            shared_ptr<Nodo> nodo_a_enlazar = _ruta.back()->obtener_nodo(nombre_nodo);
            cd(ruta_actual);
            if (nodo_a_enlazar != nullptr) {
                shared_ptr<Nodo> nodo_existente = _ruta.back()->obtener_nodo(name);
                if (nodo_existente == nullptr) {
                    shared_ptr<Enlace> nuevoEnlace = make_shared<Enlace>(Enlace(name,nodo_a_enlazar));
                    _ruta.back()->insert_elem(nuevoEnlace);
                }
                else {
                    // Error: ya existe un nodo con ese nombre en ese path
                    throw yaExisteNodo();
                }
            }
            else {
                // Error: no existe un nodo con ese nombre en ese path
                throw noExisteNodo();
            }
        }
        else {
            // el nodo a enlazar está en el directorio padre
            cd("/");
            shared_ptr<Nodo> nodo_a_enlazar = _ruta.back()->obtener_nodo(nombre_nodo);
            cd(ruta_actual);
            if (nodo_a_enlazar != nullptr) {
                shared_ptr<Nodo> nodo_existente = _ruta.back()->obtener_nodo(name);
                if (nodo_existente == nullptr) {
                    shared_ptr<Enlace> nuevoEnlace = make_shared<Enlace>(Enlace(name,nodo_a_enlazar));
                    _ruta.back()->insert_elem(nuevoEnlace);
                }
                else {
                    // Error: ya existe un nodo con ese nombre en ese path
                    throw yaExisteNodo();
                }
            }
            else {
                // Error: no existe un nodo con ese nombre en ese path
                throw noExisteNodo();
            }
        }
    }
}



/* Devuelve el tamaño del nodo que referencia el path */
int Shell::stat(string path) {
    int ult_barra = path.find_last_of('/');
    if (ult_barra == -1) {
        string nombre_nodo = path;
        shared_ptr<Nodo> nodo_referenciado = _ruta.back()->obtener_nodo(nombre_nodo);
        if (nodo_referenciado != nullptr) {
            return nodo_referenciado->get_tam();
        }
        else {
            // Error: no existe un nodo con ese nombre en ese path
            throw noExisteNodo();
        }
    }
    else if (ult_barra == (int)path.size()-1) {
        path.erase(path.end()-1);
        return stat(path);
    }
    else {
        string ruta_actual = pwd();
        string ruta_nueva = path;
        ruta_nueva.erase(ruta_nueva.begin()+ult_barra,ruta_nueva.end());
        string nombre_nodo = path;
        nombre_nodo.erase(nombre_nodo.begin(),nombre_nodo.begin()+ult_barra+1);
        
        if (ruta_nueva != "") {
            cd(ruta_nueva);
            shared_ptr<Nodo> nodo_referenciado = _ruta.back()->obtener_nodo(nombre_nodo);
            cd(ruta_actual);
            if (nodo_referenciado != nullptr) {
                return nodo_referenciado->get_tam();
            }
            else {
                // Error: no existe un nodo con ese nombre en ese path
                throw noExisteNodo();
            }
        }
        else {
            cd("/");
            shared_ptr<Nodo> nodo_referenciado = _ruta.back()->obtener_nodo(nombre_nodo);
            cd(ruta_actual);
            if (nodo_referenciado != nullptr) {
                return nodo_referenciado->get_tam();
            }
            else {
                // Error: no existe un nodo con ese nombre en ese path
                throw noExisteNodo();
            }
        }
    }
}

/* Elimina un nodo. Si es un fichero, es simplemente eliminado. Si es un enlace, elimina el 
   enlace pero no el nodo referenciado. Si es un directorio, elimina el directorio y todo su
   contenido. Si existen enlaces al elemento borrado, ese elemento sigue siendo accesible a
   través del enlace (todavía existe), pero no a través de su ubicación original (que ha sido
   eliminada). Si el elemento eliminado es un directorio o enlace a directorio que forma parte
   del path al directorio activo, no se eliminará hasta que deje de hacerlo (cambiemos el
   directorio actual a otro que no lo incluya en su path). */
void Shell::rm(string path) {
    int ult_barra = path.find_last_of('/');
    if (ult_barra == -1) {
        // en el directorio actual
        string ruta_actual = pwd();
        string nombre_nodo = path;
        shared_ptr<Nodo> nodo_a_borrar = _ruta.back()->obtener_nodo(nombre_nodo);
        if (nodo_a_borrar != nullptr) {
            shared_ptr<Directorio> nodo_directorio = dynamic_pointer_cast<Directorio>(nodo_a_borrar);
            if (nodo_directorio != nullptr) {
                // el nodo a borrar es un directorio
                cd(nombre_nodo);
                _ruta.back()->borrar_todos_nodos();
                cd(ruta_actual);
                _ruta.back()->borrar_nodo(nodo_a_borrar);
            }
            else {
                // el nodo a borrar es un fichero o un enlace
                _ruta.back()->borrar_nodo(nodo_a_borrar);
            }
        }
        else {
            // Error: no existe un nodo con ese nombre en ese path
            throw noExisteNodo();
        }
    }
    else if (ult_barra == (int)path.size()-1) {
        path.erase(path.end()-1);
        rm(path);
    }
    else {
        // en otro directorio distinto
        string ruta_actual = pwd();
        string ruta_nueva = path;
        string nombre_nodo = path;
        ruta_nueva.erase(ruta_nueva.begin()+ult_barra,ruta_nueva.end());
        nombre_nodo.erase(nombre_nodo.begin(),nombre_nodo.begin()+ult_barra+1);

        if (ruta_nueva != "") {
            cd(ruta_nueva);
            shared_ptr<Nodo> nodo_a_borrar = _ruta.back()->obtener_nodo(nombre_nodo);
            if (nodo_a_borrar != nullptr) {
                shared_ptr<Directorio> nodo_directorio = dynamic_pointer_cast<Directorio>(nodo_a_borrar);
                if (nodo_directorio != nullptr) {
                    // el nodo a borrar es un directorio
                    cd(nombre_nodo);
                    _ruta.back()->borrar_todos_nodos();
                    cd(ruta_actual);
                    _ruta.back()->borrar_nodo(nodo_a_borrar);
                }
                else {
                    // el nodo a borrar es un fichero o un enlace
                    _ruta.back()->borrar_nodo(nodo_a_borrar);
                }
            }
            else {
                // Error: no existe un nodo con ese nombre en ese path
                throw noExisteNodo();
            }
            cd(ruta_actual);
        }
        else {
            cd("/");
            shared_ptr<Nodo> nodo_a_borrar = _ruta.back()->obtener_nodo(nombre_nodo);
            if (nodo_a_borrar != nullptr) {
                shared_ptr<Directorio> nodo_directorio = dynamic_pointer_cast<Directorio>(nodo_a_borrar);
                if (nodo_directorio != nullptr) {
                    // el nodo a borrar es un directorio
                    cd(nombre_nodo);
                    _ruta.back()->borrar_todos_nodos();
                    cd(ruta_actual);
                    _ruta.back()->borrar_nodo(nodo_a_borrar);
                }
                else {
                    // el nodo a borrar es un fichero o un enlace
                    _ruta.back()->borrar_nodo(nodo_a_borrar);
                }
            }
            else {
                cd(ruta_actual);
                // Error: no existe un nodo con ese nombre en ese path
                throw noExisteNodo();
            }
        }
    }
}
