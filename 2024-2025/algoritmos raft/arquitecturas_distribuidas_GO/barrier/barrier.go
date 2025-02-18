/*
* AUTOR: Rafael Tolosana Calasanz y Unai Arronategui
* MODIFICADO POR: Irene Pascual Albericio (871627) y Ariana Porroche Llorén (874055)
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2022
* FECHA DE MODIFICACIÓN: octubre de 2024
* FICHERO: barrier.go
* DESCRIPCIÓN: barrera distribuida
 */

package main

import (
	"bufio"
	"errors"
	"fmt"
	"net"
	"os"
	"strconv"
	"sync"
	"time"
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

func getEndpoints() ([]string, int, error) {
	endpointsFile := os.Args[1]
	lineNumber, err := strconv.Atoi(os.Args[2])
	if err != nil || lineNumber < 1 {
		return nil, 0, errors.New("invalid line number")
	}

	endpoints, err := readEndpoints(endpointsFile)
	if err != nil {
		return nil, 0, err
	}

	if lineNumber > len(endpoints) {
		return nil, 0, errors.New("line number out of range")
	}

	return endpoints, lineNumber, nil
}



func handleConnection(conn net.Conn, barrierChan chan<- bool, received map[string]bool, mu *sync.Mutex, n int) {
	defer conn.Close()
	buf := make([]byte, 1024)
	_, err := conn.Read(buf)
	if err != nil {
		fmt.Println("Error reading from connection:", err)
		return
	}
	msg := string(buf)
	mu.Lock()
	defer mu.Unlock()

	received[msg] = true
	fmt.Println("Received", len(received), "elements")
	if len(received) == n-1 {
		barrierChan<-true
	}
}

func acceptAndHandleConnections(listener net.Listener, quitChannel chan bool, barrierChan chan bool, receivedMap map[string]bool, mu *sync.Mutex, n int) {
	for {
		select {
		case <-quitChannel:
			fmt.Println("Stopping the listener...")
			close(quitChannel)
			return
		default:
			conn, err := listener.Accept()
			if err != nil {
				fmt.Println("Error accepting connection:", err)
				continue
			}
			go handleConnection(conn, barrierChan, receivedMap, mu, n)
		}
	}
}

func notifyOtherDistributedProcesses(endpoints []string, lineNumber int) {
	for i, ep := range endpoints {
		if i+1 != lineNumber {
			go func(ep string) {
				for {
					conn, err := net.Dial("tcp", ep)
					if err != nil {
						fmt.Println("Error connecting to", ep, ":", err)
						time.Sleep(1 * time.Second)
						continue
					}
					_, err = conn.Write([]byte(strconv.Itoa(lineNumber)))
					if err != nil {
						fmt.Println("Error sending message:", err)
						conn.Close()
						continue
					}
					conn.Close()
					break
				}
			}(ep)
		}
	}
}

func main() {
	if len(os.Args) != 3 {
		fmt.Println("Usage: go run main.go <endpoints_file> <line_number>")
		return
	}

	endpoints, lineNumber, err := getEndpoints()
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	n := len(endpoints)
	localEndpoint := endpoints[lineNumber-1]

	listener, err := net.Listen("tcp", localEndpoint)
	if err != nil {
		fmt.Println("Error creating listener:", err)
		return
	}

	fmt.Println("Listening on", localEndpoint)

	var mu sync.Mutex
	quitChannel := make(chan bool)
	receivedMap := make(map[string]bool)
	barrierChan := make(chan bool)

	go acceptAndHandleConnections(listener, quitChannel, barrierChan, receivedMap, &mu, n)

	time.Sleep(2*time.Second)

	notifyOtherDistributedProcesses(endpoints, lineNumber)

	fmt.Println("Waiting for all the processes to reach the barrier")
	time.Sleep(2*time.Second)
	<-barrierChan

	fmt.Println("All processes reached the barrier, proceeding...")
	listener.Close()
	quitChannel<-true
}
