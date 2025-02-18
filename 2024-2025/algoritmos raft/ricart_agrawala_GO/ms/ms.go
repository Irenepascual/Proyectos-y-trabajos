/*
* AUTOR: Rafael Tolosana Calasanz
* MODIFICADO POR: Irene Pascual Albericio 871627 y Ariana Porroche Llorén 874055
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2021
* FECHA DE MODIFICACIÓN: octubre de 2024
* FICHERO: ms.go
* DESCRIPCIÓN: Implementación de un sistema de mensajería asíncrono, insipirado en el Modelo Actor
 */
package ms

import (
	"bufio"
	"encoding/gob"
	"fmt"
	"net"
	"os"
	"time"
)

type Message interface{}

type MessageSystem struct {
	mbox  chan Message
	peers []string
	done  chan bool
	me    int
}

const (
	MAXMESSAGES = 10000
)

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}

func parsePeers(path string) (lines []string) {
	file, err := os.Open(path)
	checkError(err)
	defer file.Close()
	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	return lines
}

// Pre: pid en {1..n}, el conjunto de procesos del SD
// Post: envía el mensaje msg a pid
func (ms *MessageSystem) Send(pid int, msg Message) {
	conn, err := net.Dial("tcp", ms.peers[pid-1])
	checkError(err)
	encoder := gob.NewEncoder(conn)
	err = encoder.Encode(&msg)
	conn.Close()
}

// Pre: True
// Post: el mensaje msg de algún Proceso P_j se retira del mailbox y se devuelve
//
//	Si mailbox vacío, Receive bloquea hasta que llegue algún mensaje
func (ms *MessageSystem) Receive() (msg Message) {
	msg = <-ms.mbox
	return msg
}

//	messageTypes es un slice con tipos de mensajes que los procesos se pueden intercambiar a través de este ms
//
// Hay que registrar un mensaje antes de poder utilizar (enviar o recibir)
// Notar que se utiliza en la función New
func Register(messageTypes []Message) {
	for _, msgTp := range messageTypes {
		gob.Register(msgTp)
	}
}

// Pre: whoIam es el pid del proceso que inicializa este ms
//
//	usersFile es la ruta a un fichero de texto que en cada línea contiene IP:puerto de cada participante
//	messageTypes es un slice con todos los tipos de mensajes que los procesos se pueden intercambiar a través de este ms
//
// Post: devuelve el sistema de mensajes ms, el listener de la conexión y el númeor de participantes
func New(whoIam int, usersFile string, messageTypes []Message) (ms MessageSystem, listener net.Listener, n int) {
	ms.me = whoIam
	ms.peers = parsePeers(usersFile)
	ms.mbox = make(chan Message, MAXMESSAGES)
	ms.done = make(chan bool)
	Register(messageTypes)

	listener, err := net.Listen("tcp", ms.peers[ms.me-1])
	checkError(err)
	fmt.Println("Process listening at " + ms.peers[ms.me-1])

	go func() {
		tratarMensajes(ms, listener)
		close(ms.mbox)
	}()

	return ms, listener, len(ms.peers)
}

// Pre: ms es el sistema de mensajes previamente inicializado y el listener es la conexión
// Post: espera recibir un mensaje y lo pone en el mbox, hasta que le avisen de que el canal
//
//	esté cerrado y finaliza la ejecución
func tratarMensajes(ms MessageSystem, listener net.Listener) {
	for {
		time.Sleep(2 * time.Second)
		select {
		case <-ms.done:
			return
		default:
			conn, err := listener.Accept()
			if err == nil {
				decoder := gob.NewDecoder(conn)
				var msg Message
				err = decoder.Decode(&msg)
				conn.Close()
				ms.mbox <- msg
			}
		}
	}
}

// Pre: True
// Post: termina la ejecución de este ms
func (ms *MessageSystem) Stop(listener net.Listener) {
	listener.Close()
	ms.done <- true
	fmt.Println("MS: fin de Stop()")
}
