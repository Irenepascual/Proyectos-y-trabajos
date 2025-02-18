package main

import (
	//"errors"
	"fmt"
	//"log"
	"net"
	"net/rpc"
	"os"
	"raft/internal/raft"
	"raft/internal/comun/rpctimeout"
	"raft/internal/comun/check"
	"strconv"
	//"time"
	"strings"
)


func main() {
	// obtener entero de indice de este nodo
//	me, err := strconv.Atoi(os.Args[1])
//	check.CheckError(err, "Main, mal numero entero de indice de nodo:")
	
	mestring := os.Args[1]
	nombre:=strings.Split(mestring,"-")[0]
	num,_:=strconv.Atoi(strings.Split(mestring,"-")[1])

	var nodos5 []string
	for i:=0 ; i < 3; i++{
		nodo5:=nombre+"-"+strconv.Itoa(i)+"."+"ss-service.default.svc.cluster.local:6000"
		nodos5 = append(nodos5, nodo5)
	}


	var nodos []rpctimeout.HostPort
	// Resto de argumento son los end points como strings
	// De todas la replicas-> pasarlos a HostPort
	for _, endPoint := range os.Args[2:] {
		 nodos = append(nodos, rpctimeout.HostPort(endPoint))
	}

	// Parte Servidor
	nr := raft.NuevoNodo(nodos, num,
				make(chan raft.AplicaOperacion, 1000))
	rpc.Register(nr)

	fmt.Println("Replica escucha en:", mestring, "de", nodos5[num])
	nr.Logger.Println("Replica escucha en:", mestring, "de", nodos5[num])
	l, err := net.Listen("tcp", nodos5[num])
	check.CheckError(err, "Main listen error:")
	rpc.Accept(l)
}
