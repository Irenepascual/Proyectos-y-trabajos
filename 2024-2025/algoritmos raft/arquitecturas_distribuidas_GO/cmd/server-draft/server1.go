/*
* AUTOR: Rafael Tolosana Calasanz y Unai Arronategui
* MODIFICADO POR: Irene Pascual Albericio (871627) y Ariana Porroche Llorén (874055)
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2022
* FECHA DE MODIFICACIÓN: octubre de 2024
* FICHERO: server1.go
* DESCRIPCIÓN: cliente servidor concurrente creando una Goroutine por petición
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

func processRequest(conn net.Conn){
	// 1. Esperamos a tener una petición
	var request com.Request
	decoder := gob.NewDecoder(conn)
	err := decoder.Decode(&request)
	com.CheckError(err)
	log.Println(request.Id)

	if request.Id != -1 {
		// 2. Obtenemos los números primos y los enviamos
		primes := findPrimes(request.Interval)
		reply := com.Reply{Id: request.Id, Primes: primes}
		encoder := gob.NewEncoder(conn)
		encoder.Encode(&reply)
	}
	conn.Close()
}

func main() {
	args := os.Args
	if len(args) != 2 {
		log.Println("Error: endpoint missing: go run server1.go ip:port")
		os.Exit(1)
	}
	endpoint := args[1]
	listener, err := net.Listen("tcp", endpoint)
	com.CheckError(err)

	log.SetFlags(log.Lshortfile | log.Lmicroseconds)


	log.Println("***** Listening for new connection in endpoint ", endpoint)
	for {
		conn, err := listener.Accept()
		com.CheckError(err)
		go processRequest(conn)
	}
}
