/*
* AUTOR: Irene Pascual Albericio 871627 y Ariana Porroche Llorén 874055
* FECHA: octubre de 2024
* FICHERO: lector.go
 */
package main

import (
	"fmt"
	"os"
	"practica2/cmd"
	"practica2/ra"
	"strconv"
	"time"
)

func main() {
	args := os.Args
	if len(args) != 4 {
		fmt.Println("Usage: go run main.go <pid> <ficheroParticipantes> <fichero>")
		os.Exit(1)
	}

	me, err := strconv.Atoi(args[1])
	if err != nil {
		fmt.Println("Error en el PID del proceso")
		return
	}
	ficheroParticipantes := args[2]
	ficheroTexto := args[3]

	ra, listener := ra.New(me, ficheroParticipantes, true)

	time.Sleep(7 * time.Second) // esperamos a que se conecten todos los procesos

	for i := 0; i < 2; i++ {
		fmt.Println(me, ": comienza PreProtocolo")
		ra.PreProtocol()

		fmt.Println(me, ": comieza LeerFichero")
		contenido, err := cmd.LeerFichero(ficheroTexto)
		if err != nil {
			fmt.Println("Error al leer el fichero:", ficheroTexto)
		} else {
			fmt.Println("--Fichero leido correctamente. Contenido del fichero:")
			fmt.Println(contenido)
		}

		fmt.Println(me, ": comienza PostProtocolo")
		ra.PostProtocol()
	}

	time.Sleep(9 * time.Second) // esperamos a que acaben todos los procesos

	ra.Stop(listener)
	fmt.Println(me, ": fin")
}
