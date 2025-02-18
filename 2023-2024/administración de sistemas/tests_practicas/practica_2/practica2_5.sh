#!/bin/bash
#871627, Pascual Albericio, Irene, M, 3, A
#874762, Vayaz Díaz, Iván, M, 3, A

echo -n "Introduzca el nombre de un directorio: "
read dir

numDir=0
numFich=0

if test ! -d "$dir"
then
	echo "$dir no es un directorio"
else
	for var in "$dir"/*
	do
		if test -d "$var"
		then
			numDir=$((numDir+1))
		elif test -f "$var"
		then
			numFich=$((numFich+1))
		fi
	done
	echo "El numero de ficheros y directorios en $dir es de $numFich y $numDir, respectivamente"
fi
