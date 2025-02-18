/*
* AUTOR: Irene Pascual Albericio 871627 y Ariana Porroche Llorén 874055
* FECHA: octubre de 2024
* FICHERO: escritor.go
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
	if len(args) != 5 {
		fmt.Println("Usage: go run main.go <pid> <ficheroParticipantes> <fichero> <fragmento>")
		os.Exit(1)
	}

	me, err := strconv.Atoi(args[1])
	if err != nil {
		fmt.Println("Error en el PID del proceso")
		return
	}
	ficheroParticipantes := args[2]
	ficheroTexto := args[3]
	fragmento := args[4]

	ra, listener := ra.New(me, ficheroParticipantes, false)

	time.Sleep(7 * time.Second) // esperando a que se conecten todos los procesos

	for i := 0; i < 2; i++ {
		fmt.Println(me, ": comienza PreProtocolo")
		ra.PreProtocol()

		fmt.Println(me, ": comieza EscribirFichero")
		err := cmd.EscribirFichero(ficheroTexto, fragmento)
		if err != nil {
			fmt.Println("Error al escribir el fichero:", ficheroTexto)
		} else {
			fmt.Println("--Fichero escrito correctamente")
		}

		fmt.Println(me, ": comienza PostProtocolo")
		ra.PostProtocol()
	}

	time.Sleep(9 * time.Second) // esperando a que acaben todos los procesos

	ra.Stop(listener)
	fmt.Println(me, ": fin")
}
