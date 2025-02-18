/*
* AUTOR: Rafael Tolosana Calasanz y Unai Arronategui
* MODIFICADO POR: Irene Pascual Albericio (871627) y Ariana Porroche Llorén (874055)
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2022
* FECHA DE MODIFICACIÓN: octubre de 2024
* FICHERO: master3.go
* DESCRIPCIÓN: fichero master de la arquitectura máster-worker
 */
package main

import (
	"encoding/gob"
	"log"
	"os"
	"os/exec"
	"net"
	"practica1/com"
	"bufio"
	"strings"
)

func readEndpoints(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var endpoints []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if line != "" {
			endpoints = append(endpoints, line)
		}
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return endpoints, nil
}

func crearPool(fichero string, poolChan chan net.Conn) {
	var endpoints []string
	var err error
	if endpoints, err = readEndpoints(fichero); err != nil {
		log.Println("Error: no se puede abrir el fichero", fichero)
	} else {
		for i, ep := range endpoints {
			// 1. lanzar worker
			go lanzarWorker(ep, i, poolChan)
		}
	}
}

func lanzarWorker(ep string, id int, poolChan chan net.Conn) {
	ruta := "/misc/alumnos/sd/sd2425/a874055/practica1/cmd/server-draft"
	partes := strings.Split(ep, ":")
	ip := partes[0]

		// ssh 192.168.3.9 "cd ... && go run worker3.go 192.168.3.9:29270 && exit"
	cmd := exec.Command("ssh", ip, "cd", ruta, "&&", "go", "run", "worker3.go", ep, "&&", "exit")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err := cmd.Start()
	if err != nil {
		log.Println("Error al lanzar el worker con dirección:", ip)
	} else {
		// 2. dar trabajo
		log.Println("Lanzado el worker", ep)
		handleRequest(id,ep, ip, poolChan)
	}
}

func handleRequest(id int, ep string, ip string, poolChan chan net.Conn) {
	for {
		// 1. Comprobamos la conexión con el worker
		cmd := exec.Command("ping", "-c", "1", ip)
		err := cmd.Run()
		// 1.1. Si no conexion -> salimos
		if err != nil {
			log.Println("Worker", ep, "ha dejado de responder")
			break
		}

		// 1.2. Si si conexion -> esperamos a recibir cliente
		conn := <- poolChan

		// 2. Esperamos a recibir petición del cliente
		encoder := gob.NewEncoder(conn)
		decoder := gob.NewDecoder(conn)
		var request com.Request
		err = decoder.Decode(&request)
		com.CheckError(err)
		log.Println("M1: Recibida peticion", request.Id, "del cliente")

		if request.Id != -1 {
			// 3. Nos conectamos con el worker
			conn2, err := net.Dial("tcp", ep)
			com.CheckError(err)
			log.Println("M2: Establecida la conexion con el worker", id, "en la dir", ep)

			// 4. Enviamos al worker el intervalo
			encoder2 := gob.NewEncoder(conn2)
			decoder2 := gob.NewDecoder(conn2)
			err = encoder2.Encode(request)
			com.CheckError(err)

			// 5. Recibimos la respuesta del worker
			var reply com.Reply
			err = decoder2.Decode(&reply)
			com.CheckError(err)
			log.Println("M3: Recibida respuesta del worker", id, "->", reply.Id)

			conn2.Close()

			// 5. Enviamos la respuesta al cliente
			primos_reply := com.Reply{Id: request.Id, Primes: reply.Primes}
			err = encoder.Encode(primos_reply)
			com.CheckError(err)
			log.Println("M4: Enviada respuesta al cliente")
		}
		conn.Close()
	}
}


func main() {
	args := os.Args
	if len(args) != 3 {
		log.Println("Error: endpoint missing: go run master3.go <ip:port> <fich>")
		os.Exit(1)
	}
	endpoint := args[1]
	fichero := args[2]
	var poolChan = make(chan net.Conn)

	listener, err := net.Listen("tcp", endpoint)
	com.CheckError(err)

	log.SetFlags(log.Lshortfile | log.Lmicroseconds)
	log.Println("***** Listening for new connection in endpoint ", endpoint)

	go crearPool(fichero, poolChan)

	for {
		conn, err := listener.Accept()
		com.CheckError(err)
		poolChan <- conn
	}
}
