#!/bin/bash
#874762, Vayad Díaz, Iván, M, 3, A
#871627, Pascual Albericio, Irene, M, 3, A

echo -n "Introduzca una tecla: "
IFS= read -rsn1 tecla

if test ! -z "$tecla"
then
	case $tecla in
		[a-zA-Z]*)
			echo "$tecla es una letra"
			;;
		[0-9]*)
			echo "$tecla es un numero"
			;;
		
		*)
			echo "$tecla es un caracter especial"
			;;
	esac
fi