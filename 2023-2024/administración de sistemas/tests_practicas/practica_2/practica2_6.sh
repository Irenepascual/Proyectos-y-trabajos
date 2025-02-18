#!/bin/bash
#871627, Pascual Albericio, Irene, M, 3, A 
#874762, Vayaz Díaz, Iván, M, 3, A

user_home=$(eval echo ~$USER)

# Encontrar todos los directorios que sigan el patrón binXXX en el directorio raíz del usuario
matching_dirs=$(find "$user_home" -maxdepth 1 -type d -name 'bin*' -exec stat -c '%n,%Y' {} + | sort -t',' -k2 | awk -F',' '{print $1}' | head -n 1)

# Si no se encontraron directorios que sigan el patrón binXXX, crear uno nuevo
if [ -z "$matching_dirs" ]
then 
	temp_dir=$(mktemp -d "$user_home/binXXX") 
	echo "Se ha creado el directorio $temp_dir"
else 
	temp_dir="$matching_dirs" 
fi

echo "Directorio destino de copia: $temp_dir"
copias=0

for var in *
do
	if test -f "$var" -a -x "$var"
	then
		cp "$var" "$temp_dir"
		echo "./$var ha sido copiado a $temp_dir"
		copias=$((copias+1))
	fi
done

if test $copias -eq 0
then
        echo "No se ha copiado ningun archivo"
elif test  $copias = "1"
then
        echo "Se ha copiado $copias archivo"
else
        echo "Se han copiado $copias archivos"
fi
