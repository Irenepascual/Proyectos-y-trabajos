package main

import (
	"fmt"
	"raft/internal/comun/check"
	"raft/internal/comun/rpctimeout"
	"raft/internal/raft"
	"strconv"
	"time"
)

func pruebaUnLider(nodos []rpctimeout.HostPort, numreplicas int) int {
	var reply raft.EstadoRemoto
	mapaLideres := make(map[int][]int)

	for iters := 0; iters < 10; iters++ {
		time.Sleep(2000 * time.Millisecond)
		for i := 0; i < numreplicas; i++ {
			err := nodos[i].CallTimeout("NodoRaft.ObtenerEstadoNodo",
				raft.Vacio{}, &reply, 500*time.Millisecond)
			check.CheckError(err, "Error en llamada RPC ObtenerEstadoRemoto")
			fmt.Println("reply de ", i, "ReplyMandato: ", reply.Mandato, "es Lider: ", reply.EsLider)
			if reply.EsLider {
				mapaLideres[reply.Mandato] = append(mapaLideres[reply.Mandato], i)
				break
			}
		}

		ultimoMandatoConLider := -1
		for mandato, lideres := range mapaLideres {
			if len(lideres) > 1 {
				fmt.Errorf("Error -> hay más de 1 líder")
				return -1
			}
			if mandato > ultimoMandatoConLider {
				ultimoMandatoConLider = mandato
			}
		}

		if len(mapaLideres) != 0 {
			fmt.Println("Encontrado el líder", mapaLideres[ultimoMandatoConLider][0])
			return mapaLideres[ultimoMandatoConLider][0]
		}
	}
	return -1
}

func someterOperacion(op raft.TipoOperacion, numOp int, idLider int, nodos []rpctimeout.HostPort) {
	fmt.Println("Vamos a someter la operación", numOp, "al líder", idLider)
	var reply raft.ResultadoRemoto
	err := nodos[idLider].CallTimeout("NodoRaft.SometerOperacionRaft", op,
		&reply, 5000*time.Millisecond)
	check.CheckError(err, "Error en la llamada RPC SometerOperacion")
	if reply.ValorADevolver == "5" {
		fmt.Println("Operación", numOp, "sometida correctamente, valor devuelto:",
			reply.ValorADevolver)
	} else {
		fmt.Println("Operación", numOp, "sometida incorrectamente, valor devuelto:",
			reply.ValorADevolver)
	}
}

func pararNodoNoLider(idLider int, nodos []rpctimeout.HostPort) {
	var reply raft.ResultadoRemoto
	for i := 0; i < len(nodos); i++ {
		if i != idLider {
			fmt.Println("Vamos a matar a la réplica", i)
			err := nodos[i].CallTimeout("NodoRaft.ParaNodo", raft.Vacio{},
				&reply, 20*time.Millisecond)
			check.CheckError(err, "ParaNodo")
			break
		}
	}
}

func main() {
	fmt.Println("Entrando en el main del cliente")
	var nodes5 []string
	for i := 0; i < 3; i++ {
		node5 := "ss-" + strconv.Itoa(i) + ".ss-service.default.svc.cluster.local:6000"
		nodes5 = append(nodes5, node5)
	}

	var nodos []rpctimeout.HostPort
	for _, endPoint := range nodes5 {
		nodos = append(nodos, rpctimeout.HostPort(endPoint))
	}

	idLider := pruebaUnLider(nodos, 3)
	if idLider == -1 {
		return
	}
	fmt.Println("Ya se ha elegido el líder: ", idLider)

	fmt.Println("Vamos a someter la operación 1 al líder", idLider)
	op1 := raft.TipoOperacion{"escribir", "var1", "5"}
	someterOperacion(op1, 1, idLider, nodos)

	pararNodoNoLider(idLider, nodos)

	fmt.Println("Vamos a someter la operación 2 al líder", idLider)
	op2 := raft.TipoOperacion{"leer", "var1", ""}
	someterOperacion(op2, 2, idLider, nodos)
}