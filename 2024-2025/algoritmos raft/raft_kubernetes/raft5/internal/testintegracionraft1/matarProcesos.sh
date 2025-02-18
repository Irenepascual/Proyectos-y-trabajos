#!/bin/bash

# Filtrar los procesos y extraer los PIDs usando grep y awk
pids=$(ps aux | grep '^a874055' | grep -E 'go run|/tmp/' | awk '{print $2}')

# Comprobar si hay procesos a matar
if [[ -z "$pids" ]]; then
    echo "No se encontraron procesos para matar."
else
    echo "Matando los siguientes procesos: $pids"
	for pid in $pids; do
		echo "Matando el proceso $pid"
		kill -9 $pid
	done
    echo "Todos los procesos matados"
fi
