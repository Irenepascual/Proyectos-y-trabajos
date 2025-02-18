/*
* AUTOR: Rafael Tolosana Calasanz y Unai Arronategui
* MODIFICADO POR: Irene Pascual Albericio (871627) y Ariana Porroche Llorén (874055)
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2022
* FECHA DE MODIFICACIÓN: octubre de 2024
* FICHERO: server2.go
* DESCRIPCIÓN: cliente servidor concurrente con un pool fijo de Goroutines
 */
package main

import (
	"encoding/gob"
	"log"
	"os"
	"net"
	"practica1/com"
)

// PRE: verdad = !foundDivisor
// POST: IsPrime devuelve verdad si n es primo y falso en caso contrario
func isPrime(n int) (foundDivisor bool) {
	foundDivisor = false
	for i := 2; (i < n) && !foundDivisor; i++ {
		foundDivisor = (n%i == 0)
	}
	return !foundDivisor
}

// PRE: interval.A < interval.B
// POST: FindPrimes devuelve todos los números primos comprendidos en el
//
//	intervalo [interval.A, interval.B]
func findPrimes(interval com.TPInterval) (primes []int) {
	for i := interval.Min; i <= interval.Max; i++ {
		if isPrime(i) {
			primes = append(primes, i)
		}
	}
	return primes
}

func poolRoutine(connChan chan net.Conn) {
	for {
		// 1. Esperamos a tener una petición y la leemos
		conn :=  <- connChan
		var request com.Request
	        decoder := gob.NewDecoder(conn)
        	err := decoder.Decode(&request)
        	com.CheckError(err)

		// 2. Procesamos la petición
		processRequest(conn, request)
		conn.Close()
	}
}

func processRequest(conn net.Conn, request com.Request){
	log.Println("recibida peticion ", request.Id)

	if request.Id != -1 {
		// Obtenemos los números primos y los enviamos
		primes := findPrimes(request.Interval)
		reply := com.Reply{Id: request.Id, Primes: primes}
		encoder := gob.NewEncoder(conn)
		encoder.Encode(&reply)
	}
}

func main() {
	args := os.Args
	if len(args) != 2 {
		log.Println("Error: endpoint missing: go run server2.go ip:port")
		os.Exit(1)
	}
	endpoint := args[1]
	listener, err := net.Listen("tcp", endpoint)
	com.CheckError(err)

	log.SetFlags(log.Lshortfile | log.Lmicroseconds)
	log.Println("***** Listening for new connection in endpoint ", endpoint)

	poolChan := make(chan net.Conn)
	nPOOL := 6
	for i := 0; i < nPOOL; i++ {
		go poolRoutine(poolChan)
	}

	for {
		conn, err := listener.Accept()
		com.CheckError(err)
		poolChan <- conn
	}
}
