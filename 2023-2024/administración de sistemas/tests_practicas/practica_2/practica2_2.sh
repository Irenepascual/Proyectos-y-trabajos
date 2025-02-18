#!/bin/bash
#874762, Vayad Diaz, Ivan, M, 3, A
#871627, Pascual Albericio, Irene, M, 3, A

for i in "$@"
do
	if test -f "$i"
	then
		more "$i"
	else
		echo "$i no es un fichero"
	fi
done 
