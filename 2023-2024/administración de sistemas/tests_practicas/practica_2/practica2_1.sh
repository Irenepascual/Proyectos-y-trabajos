#!/bin/bash
#874762, Vayad Diaz, Ivan, M, 3, A
#871627, Pascual Albericio, Irene, M, 3, A

echo -n "Introduzca el nombre del fichero: "
read  RESPUESTA

lectura="-"
escritura="-"
ejecuta="-"

if test -f "$RESPUESTA" 
then
	if test -r "$RESPUESTA"
	then
		lectura="r"
	fi

	if test -w "$RESPUESTA"
	then
		escritura="w"
	fi

	if test -x "$RESPUESTA"
	then
		ejecuta="x"
	fi

	echo "Los permisos del archivo $RESPUESTA son: $lectura$escritura$ejecuta" 
else 
	echo "$RESPUESTA no existe"
fi
