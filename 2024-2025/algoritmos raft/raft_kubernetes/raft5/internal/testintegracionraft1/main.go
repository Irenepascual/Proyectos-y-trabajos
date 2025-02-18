package main

import (
	"fmt"
	"raft/internal/comun/check"

	"log"
	//"crypto/rand"
	//"os"
	//"path/filepath"
	"strconv"
	//"testing"
	"time"

	"raft/internal/comun/rpctimeout"
	"raft/internal/despliegue"
	"raft/internal/raft"
)

const (
	//nodos replicas
	REPLICA1 = "127.0.0.1:29270"
	REPLICA2 = "127.0.0.1:29271"
	REPLICA3 = "127.0.0.1:29272"

	// paquete main de ejecutables relativos a directorio raiz de modulo
	EXECREPLICA = "cmd/srvraft/main.go"
)

// PATH de los ejecutables de modulo golang de servicio Raft
var PATH string = "/misc/alumnos/sd/sd2425/a874055/practica3/raft5"

var EXECREPLICACMD string = "cd " + PATH + "; go run " + EXECREPLICA

//////////////////////////////////////////////////////////////////////////////
///////////////////////			 FUNCIONES TEST
/////////////////////////////////////////////////////////////////////////////

func main() {
	// <setup code>
	// Crear canal de resultados de ejecuciones ssh en maquinas remotas
	cfg := makeCfgDespliegue(3,
		[]string{REPLICA1, REPLICA2, REPLICA3},
		[]bool{true, true, true})

	// tear down code
	// eliminar procesos en máquinas remotas
	defer cfg.stop()

	// 1. Comprueba que se creen bien las réplicas
	fmt.Println("creando las réplicas y comprobando el estado .....................")

	// Poner en marcha replicas en remoto con un tiempo de espera incluido
	cfg.startDistributedProcesses()

	time.Sleep(1800 * time.Millisecond)

	// Comprobar estado replica 0
	cfg.comprobarEstadoRemoto(0, 0, false, -1)

	// Comprobar estado replica 1
	cfg.comprobarEstadoRemoto(1, 0, false, -1)

	// Comprobar estado replica 2
	cfg.comprobarEstadoRemoto(2, 0, false, -1)

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses()

	fmt.Println("............. OKEY")
}


// ---------------------------------------------------------------------
//
// Canal de resultados de ejecución de comandos ssh remotos
type canalResultados chan string

func (cr canalResultados) stop() {
	close(cr)

	// Leer las salidas obtenidos de los comandos ssh ejecutados
	for s := range cr {
		fmt.Println(s)
	}
}

// ---------------------------------------------------------------------
// Operativa en configuracion de despliegue y pruebas asociadas
type configDespliegue struct {
	conectados  []bool
	numReplicas int
	nodosRaft   []rpctimeout.HostPort
	cr          canalResultados
	// NUEVOO
	idLider int
}

// Crear una configuracion de despliegue
func makeCfgDespliegue(n int, nodosraft []string,
	conectados []bool) *configDespliegue {
	cfg := &configDespliegue{}
	cfg.conectados = conectados
	cfg.numReplicas = n
	cfg.nodosRaft = rpctimeout.StringArrayToHostPortArray(nodosraft)
	cfg.cr = make(canalResultados, 2000)

	// NUEVOO
	cfg.idLider = -1

	return cfg
}

func (cfg *configDespliegue) stop() {
	//cfg.stopDistributedProcesses()

	time.Sleep(50 * time.Millisecond)

	cfg.cr.stop()
}


// --------------------------------------------------------------------------
// FUNCIONES DE APOYO
// --------------------------------------------------------------------------

// Comprobar que hay un solo lider
// probar varias veces si se necesitan reelecciones
func (cfg *configDespliegue) pruebaUnLider(numreplicas int) int {
	for iters := 0; iters < 10; iters++ {
		time.Sleep(2000 * time.Millisecond)
		mapaLideres := make(map[int][]int)
		for i := 0; i < numreplicas; i++ {
			if cfg.conectados[i] {
				if _, mandato, eslider, _ := cfg.obtenerEstadoRemoto(i); eslider {
					mapaLideres[mandato] = append(mapaLideres[mandato], i)
				}
			}
		}

		ultimoMandatoConLider := -1
		for mandato, lideres := range mapaLideres {
			if len(lideres) > 1 {
				log.Fatalf("mandato %d tiene %d (>1) lideres",
					mandato, len(lideres))
			}
			if mandato > ultimoMandatoConLider {
				ultimoMandatoConLider = mandato
			}
		}

		if len(mapaLideres) != 0 {
			cfg.idLider = mapaLideres[ultimoMandatoConLider][0]
			return mapaLideres[ultimoMandatoConLider][0] // Termina
		}
	}
	log.Fatalf("un lider esperado, ninguno obtenido")

	return -1 // Termina
}

func (cfg *configDespliegue) obtenerEstadoRemoto(indiceNodo int) (int, int, bool, int) {
	var reply raft.EstadoRemoto
	err := cfg.nodosRaft[indiceNodo].CallTimeout("NodoRaft.ObtenerEstadoNodo",
		raft.Vacio{}, &reply, 500*time.Millisecond)
	check.CheckError(err, "Error en llamada RPC ObtenerEstadoRemoto")

	fmt.Println("test...: reply:", reply)
	return reply.IdNodo, reply.Mandato, reply.EsLider, reply.IdLider
}

// start  gestor de vistas; mapa de replicas y maquinas donde ubicarlos;
// y lista clientes (host:puerto)
func (cfg *configDespliegue) startDistributedProcesses() {
	fmt.Println("Before start following distributed processes: ", cfg.nodosRaft)

	for i, endPoint := range cfg.nodosRaft {
		despliegue.ExecMutipleHosts(EXECREPLICACMD+
			" "+strconv.Itoa(i)+" "+
			rpctimeout.HostPortArrayToString(cfg.nodosRaft),
			[]string{endPoint.Host()}, cfg.cr)

		cfg.conectados[i] = true

		// dar tiempo para se establezcan las replicas
		time.Sleep(1000 * time.Millisecond)
	}

	// aproximadamente 500 ms para cada arranque por ssh en portatil
	//	time.Sleep(1500 * time.Millisecond)
}

func (cfg *configDespliegue) stopDistributedProcesses() {
	var reply raft.Vacio

	for i, endPoint := range cfg.nodosRaft {
		if cfg.conectados[i] {
			err := endPoint.CallTimeout("NodoRaft.ParaNodo",
				raft.Vacio{}, &reply, 10*time.Millisecond)
			check.CheckError(err, "Error en llamada RPC Para nodo")
		}
	}
}

// Comprobar estado remoto de un nodo con respecto a un estado prefijado
func (cfg *configDespliegue) comprobarEstadoRemoto(idNodoDeseado int,
	mandatoDeseado int, esLiderDeseado bool, IdLiderDeseado int) {
	idNodo, mandato, esLider, idLider := cfg.obtenerEstadoRemoto(idNodoDeseado)

	fmt.Println("Estado replica 0: ", idNodo, mandato, esLider, idLider, "\n")

	if idNodo != idNodoDeseado || mandato != mandatoDeseado ||
		esLider != esLiderDeseado || idLider != IdLiderDeseado {
		log.Fatalf("Estado incorrecto en replica %d",
			idNodoDeseado)
	}

}
