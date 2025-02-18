/*
* AUTOR: Rafael Tolosana Calasanz
* MODIFICADO POR: Irene Pascual Albericio 871627 y Ariana Porroche Llorén 874055
* ASIGNATURA: 30221 Sistemas Distribuidos del Grado en Ingeniería Informática
*			Escuela de Ingeniería y Arquitectura - Universidad de Zaragoza
* FECHA: septiembre de 2021
* FECHA DE MODIFICACIÓN: octubre de 2024
* FICHERO: ricart-agrawala.go
* DESCRIPCIÓN: Implementación del algoritmo de Ricart-Agrawala Generalizado en Go
 */
package ra

import (
	"fmt"
	"net"
	"practica2/cmd"
	"practica2/ms"
	"sync"
)

type Request struct {
	Clock []int
	Pid   int
	Lw    cmd.LectorWriter
}

type Reply struct{}

type RASharedDB struct {
	OurSeqNum []int             // lrd
	HigSeqNum []int             // clock
	OutRepCnt int               // waiting_from
	ReqCS     bool              // cs_state
	RepDefd   []bool            // perm_delayed(i)
	ms        *ms.MessageSystem // mensaje
	done      chan bool         // canal de fin de perm_delayed(i)
	chrep     chan bool         // canal de fin de waiting_from(i)
	Mutex     sync.Mutex
	// NUEVO
	n     int              // número de participantes
	myPid int              // pid de mi proceso
	lw    cmd.LectorWriter // ¿soy lector o escritor?
}

// ***********************************************************************************************************

func New(me int, usersFile string, soyLector bool) (*RASharedDB, net.Listener) {
	messageTypes := []ms.Message{Request{}, Reply{}}
	msgs, listener, n := ms.New(me, usersFile, messageTypes)
	lw := cmd.New(soyLector)
	ra := RASharedDB{make([]int, n), make([]int, n), 0, false, make([]bool, n), &msgs, make(chan bool), make(chan bool), sync.Mutex{}, n, me, lw}

	go func() {
		for {
			select {
			case <-ra.done:
				fmt.Println("RA: acabando ra")
				return
			default:
				msg := ra.ms.Receive()
				// when REQUEST(k, j) is received do
				if request, ok := msg.(Request); ok {
					fmt.Println("RA: recibida REQUEST clock:", request.Clock, "pid:", request.Pid, "lw:", request.Lw.ImLector, "(mi lrd", ra.OurSeqNum, ") (mi clock", ra.HigSeqNum, ") (mi lw", ra.lw.ImLector, ")")
					ra.RequestReceived(request)
					// when PERMISSION(j) is received do
				} else if _, ok := msg.(Reply); ok {
					fmt.Println("RA: recibida PERMISSION")
					ra.PermissionReceived()
				}
			}
		}
	}()
	return &ra, listener
}

// ***********************************************************************************************************

func (ra *RASharedDB) PermissionReceived() {
	// SC
	ra.Mutex.Lock()

	// (15) waiting_from <- waiting_from \ {j}
	ra.OutRepCnt--
	if ra.OutRepCnt == 0 {
		ra.chrep <- true
	}

	// fin SC
	ra.Mutex.Unlock()
}

func (ra *RASharedDB) RequestReceived(request Request) {
	// SC
	ra.Mutex.Lock()

	// (10) clock[] <- max(clock[], k)
	for i := 0; i < ra.n; i++ {
		ra.HigSeqNum[i] = max(ra.HigSeqNum[i], request.Clock[i])
	}

	// (11) prio <- cs_state && exclude(op1, op2) && ((lrd[],i) < (k[],j))
	prio := false
	if ra.ReqCS {
		fmt.Println("RA: lw1(ra):", ra.lw.ImLector, "lw2(request):", request.Lw.ImLector)
		if !cmd.Exclude(ra.lw, request.Lw) {
			fmt.Println("RA: los 2 son lectores")
		} else {
			if ra.OurSeqNum[ra.myPid-1] < request.Clock[request.Pid-1] {
				prio = true
			} else if ra.OurSeqNum[ra.myPid-1] == request.Clock[request.Pid-1] {
				if ra.myPid < request.Pid {
					prio = true
				}
			}
		}
	}

	// fin SC
	ra.Mutex.Unlock()

	// (12) if prio then
	if prio {
		// perm_delayed[i] <- perm_delayed[i] U {j}
		ra.RepDefd[request.Pid-1] = true
		fmt.Println("RA: añadido el proceso", request.Pid, "a perm_delayed")
	} else {
		// send PERMISSION(i) to pj
		ra.ms.Send(request.Pid, Reply{})
		fmt.Println("RA: enviado PERMISSION a", request.Pid)
	}
}

func exclude(lw1 bool, lw2 bool) bool {
	if lw1 && lw2 {
		return false
	} else {
		return true
	}
}

// ***********************************************************************************************************

// Pre: Verdad
// Post: Realiza  el  PreProtocol  para el  algoritmo de
//
//	Ricart-Agrawala Generalizado
func (ra *RASharedDB) PreProtocol() {
	// SC
	ra.Mutex.Lock()
	// (1) cs_state <- trying
	ra.ReqCS = true
	// (2) lrd[i] <- clock[i] + 1
	ra.HigSeqNum[ra.myPid-1]++
	ra.OurSeqNum[ra.myPid-1] = ra.HigSeqNum[ra.myPid-1]
	fmt.Println("RA: nuevo clock:", ra.HigSeqNum, "nuevo lrd:", ra.OurSeqNum)

	// fin SC
	ra.Mutex.Unlock()

	// (3) waiting_from <- ri <- N-1
	ri := ra.n
	ra.OutRepCnt = ri - 1

	// (4) for each j in ri do
	for i := 1; i <= ri; i++ {
		if i != ra.myPid {
			// send REQUEST(lrd[], i, LECTORWRITER) to pj
			ra.ms.Send(i, Request{ra.OurSeqNum, ra.myPid, ra.lw})
			fmt.Println("RA: enviada REQUEST a", i, "lrd:", ra.OurSeqNum, "i:", ra.myPid, "lw:", ra.lw.ImLector)
		}
	}

	// (5) wait (waiting_from = 0)
	<-ra.chrep
	// (6) cs_state(i) <- in
	// ya está indicado anteriormente en ra.ReqCS[ra.myPid-1] = true
}

// ***********************************************************************************************************

// Pre: Verdad
// Post: Realiza  el  PostProtocol  para el  algoritmo de
//
//	Ricart-Agrawala Generalizado
func (ra *RASharedDB) PostProtocol() {
	// SC
	ra.Mutex.Lock()

	// (7) cs_state <- out
	ra.ReqCS = false

	// (8) for each j in perm_delayed[] do
	ri := ra.n
	for i := 1; i <= ri; i++ {
		if i != ra.myPid {
			if ra.RepDefd[i-1] {
				// perm_delayed[i] <- false
				ra.RepDefd[i-1] = false
				// send PERMISSION(i) to pj
				ra.ms.Send(i, Reply{})
				fmt.Println("RA: enviado PERMISSION a", i)
			}
		}
	}

	// fin SC
	ra.Mutex.Unlock()

	// (9) perm_delayed[] <- 0
	// ya estan todo perm_delayed a 0 tras el for
	fmt.Println("RA: fin del PostProtocolo")
}

func (ra *RASharedDB) Stop(listener net.Listener) {
	ra.ms.Stop(listener)
	ra.done <- true
	fmt.Println("RA: fin de Stop()")
}

// *******************************************************************
// * FUNCIONES AUXILIARES

// Pre: len(ourSeqNum) == len(clock)
// Post: Devuelve como primer booleano true si el reloj ourSeqNum[]
//
//	es menor que clock[] (todas sus componentes son menores o iguales
//	y al menos una es menor estricto). En caso contrario, devuelve false.
//	Devuelve como segundo booleano true si los 2 vectores son iguales,
//	si no, devuelve false
func compararRelojes(ourSeqNum []int, clock []int) (bool, bool) {
	hayAlgunMenor := false
	hayAlgunMayor := false
	sonIguales := true
	for i := 0; i < len(ourSeqNum); i++ {
		if !hayAlgunMayor {
			if ourSeqNum[i] < clock[i] {
				hayAlgunMenor = true
				sonIguales = false
			} else if ourSeqNum[i] > clock[i] {
				hayAlgunMayor = true
				sonIguales = false
			}
		}
	}
	return hayAlgunMenor && !hayAlgunMayor, sonIguales
}

// Pre: Verdad
// Post: Devuelve el máximo entre a y b
func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}
