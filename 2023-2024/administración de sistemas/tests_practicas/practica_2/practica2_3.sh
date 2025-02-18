#!/bin/bash
#874762, Vayad Diaz, Ivan, M, 3, A
#871627, Pascual Albericio, Irene, M, 3, A

arch=$1

if test "$#" -ne 1
then
	echo "Sintaxis: practica2_3.sh <nombre_archivo>"
	exit 1
else
	if test -e "$arch"
	then
		if test -f "$arch"
		then
			chmod ug+x "$arch"
			modo=$(stat -c "%A" "$1")
			echo "$modo"
			exit 1
		fi
	else
		echo "$arch no existe"
		exit 1
	fi
fi