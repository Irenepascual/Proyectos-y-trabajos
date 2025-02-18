#pragma once
#include "Directorio.h"
#include "Fichero.h"
#include "Enlace.h"
#include "excepciones.h"

class Shell {
   public:
      vector<shared_ptr<Directorio>> _ruta;
   public:
      Shell() 
      {
         shared_ptr<Directorio> dir = make_shared<Directorio>(Directorio("root",0));
         _ruta.push_back(dir);
      }

      ~Shell() {}

      /* Devuelve la representación textual de la ruta desde la raiz al directorio actual de trabajo,
         con todos los nombres de los directorios concatenados y separados por '/' */
      string pwd();

      /* Devuelve un listado con el nombre de todos los nodos contenidos en el
         directorio actual, uno por línea */
      string ls();

      /* Devuelve un listado con el nombre y el tamaño de todos los nodos contenidos
         en el directorio actual, uno por linea */
      string du();

      /* Edita el fichero de nombre 'name' (en el directorio actual). Para simular la edición, 
      simplemente se cambia el tamaño del fichero al valor especificado como parámetro. 
      Si el fichero no existe, se debe crear con el nombre y tamaño especificados */
      void vi(string name, int size);

      /* Crea un directorio de nombre 'name' en el directorio activo */
      void mkdir(string name);

      /* Cambia el directorio de trabajo (directorio actual), es decir, hace que la ruta activa cambie
         a otro directorio. La nueva ruta activa definida en 'path' debe determinar un directorio o un
         enlace a un directorio */
      /* OBSERVACIÓN: Dada la existencia de enlaces (también entre directorios), la ruta para llegar a
         un nodo determinado puede no ser única (ie, para llegar a un nodo no es obligatorio pasar por
         su padre directo en el árbol de directorios). El camino seguido para llegar a un directorio
         puede incluir enlaces, y no debe cambiar, por ejemplo, el cado de la orden 'cd ..' */
      void cd(string path);

      /* Crea en el directorio actual un enlace simbólico de nombre 'name' que apunta
         al elemento identificado mediante la ruta especificada en 'path', que a su vez
         puede ser de cualquier tipo (fichero, directorio u otro enlace). El nombre 'name'
         es un nombre simple de nodo (se creará en el directorio activo), por lo que no
         puede contener una ruta completa. La ruta definida en 'path' sí, de tal modo que
         se pueda crear un enlace a un elemento en otro directorio del árbol, que deberá
         existir previamente */
      void ln(string path, string name);

      /* Devuelve el tamaño del nodo que referencia el path */
      int stat(string path);

      /* Elimina un nodo. Si es un fichero, es simplemente eliminado. Si es un enlace, elimina el 
         enlace pero no el nodo referenciado. Si es un directorio, elimina el directorio y todo su
         contenido. Si existen enlaces al elemento borrado, ese elemento sigue siendo accesible a
         través del enlace (todavía existe), pero no a través de su ubicación original (que ha sido
         eliminada). Si el elemento eliminado es un directorio o enlace a directorio que forma parte
         del path al directorio activo, no se eliminará hasta que deje de hacerlo (cambiemos el
         directorio actual a otro que no lo incluya en su path). */
      void rm(string path); 
};