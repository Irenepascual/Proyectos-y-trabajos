package testintegracionraft1

import (
	"fmt"
	"raft/internal/comun/check"

	//"log"
	//"crypto/rand"
	//"os"
	//"path/filepath"
	"strconv"
	"testing"
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

	//REPLICA1 = "192.168.3.9:29270"
	//REPLICA2 = "192.168.3.10:29271"
	//REPLICA3 = "192.168.3.11:29272"


	// paquete main de ejecutables relativos a directorio raiz de modulo
	EXECREPLICA = "cmd/srvraft/main.go")

// PATH de los ejecutables de modulo golang de servicio Raft
var PATH string = "/misc/alumnos/sd/sd2425/a874055/practica3/raft-entradas"

// go run cmd/srvraft/main.go 0 127.0.0.1:29001 127.0.0.1:29002 127.0.0.1:29003
var EXECREPLICACMD string = "cd " + PATH + "; go run " + EXECREPLICA

//////////////////////////////////////////////////////////////////////////////
///////////////////////			 FUNCIONES TEST
/////////////////////////////////////////////////////////////////////////////

// TEST primer rango
func TestPrimerasPruebas(t *testing.T) { // (m *testing.M) {
	// <setup code>
	// Crear canal de resultados de ejecuciones ssh en maquinas remotas
	cfg := makeCfgDespliegue(t,
		3,
		[]string{REPLICA1, REPLICA2, REPLICA3},
		[]bool{true, true, true})

	// tear down code
	// eliminar procesos en máquinas remotas
	defer cfg.stop()

	// Run test sequence

	// Test1 : No debería haber ningun primario, si SV no ha recibido aún latidos
	t.Run("T1:soloArranqueYparada",
		func(t *testing.T) { cfg.soloArranqueYparadaTest1(t) })

	// Test2 : No debería haber ningun primario, si SV no ha recibido aún latidos
	t.Run("T2:ElegirPrimerLider",
		func(t *testing.T) { cfg.elegirPrimerLiderTest2(t) })

	// Test3: tenemos el primer primario correcto
	t.Run("T3:FalloAnteriorElegirNuevoLider",
		func(t *testing.T) { cfg.falloAnteriorElegirNuevoLiderTest3(t) })

}

// TEST primer rango
func TestAcuerdosConFallos(t *testing.T) { // (m *testing.M) {
	// <setup code>
	// Crear canal de resultados de ejecuciones ssh en maquinas remotas
	cfg := makeCfgDespliegue(t,
		3,
		[]string{REPLICA1, REPLICA2, REPLICA3},
		[]bool{true, true, true})

	// tear down code
	// eliminar procesos en máquinas remotas
	defer cfg.stop()


	// Test4: Tres operaciones comprometidas en configuración estable
	t.Run("T4:tresOperacionesComprometidasEstable",
		func(t *testing.T) { cfg.tresOperacionesComprometidasEstable(t) })

	// Test5: Se consigue acuerdo a pesar de desconexiones de seguidor
	t.Run("T5:AcuerdoAPesarDeDesconexionesDeSeguidor ",
		func(t *testing.T) { cfg.AcuerdoApesarDeSeguidor(t) })


	t.Run("T6:SinAcuerdoPorFallos ",
		func(t *testing.T) { cfg.SinAcuerdoPorFallos(t) })

	t.Run("T7:SometerConcurrentementeOperaciones ",
		func(t *testing.T) { cfg.SometerConcurrentementeOperaciones(t) })

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
	t           *testing.T
	conectados  []bool
	numReplicas int
	nodosRaft   []rpctimeout.HostPort
	cr          canalResultados
	// NUEVOO
	idLider int
}

// Crear una configuracion de despliegue
func makeCfgDespliegue(t *testing.T, n int, nodosraft []string,
	conectados []bool) *configDespliegue {
	cfg := &configDespliegue{}
	cfg.t = t
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
// FUNCIONES DE SUBTESTS

// Se ponen en marcha replicas - 3 NODOS RAFT
func (cfg *configDespliegue) soloArranqueYparadaTest1(t *testing.T) {
	t.Skip("SKIPPED soloArranqueYparadaTest1")

	fmt.Println(t.Name(), ".....................")

	cfg.t = t // Actualizar la estructura de datos de tests para errores

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

	fmt.Println(".............", t.Name(), "Superado")
}

// Primer lider en marcha - 3 NODOS RAFT
func (cfg *configDespliegue) elegirPrimerLiderTest2(t *testing.T) {
	t.Skip("SKIPPED ElegirPrimerLiderTest2")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	time.Sleep(1500 * time.Millisecond)

	// Se ha elegido lider ?
	fmt.Printf("Probando lider en curso\n")
	cfg.pruebaUnLider(3)

	// Parar réplicas alamcenamiento en remoto
	cfg.stopDistributedProcesses() // Parametros

	fmt.Println(".............", t.Name(), "Superado")
}

// Fallo de un primer lider y reeleccion de uno nuevo - 3 NODOS RAFT
func (cfg *configDespliegue) falloAnteriorElegirNuevoLiderTest3(t *testing.T) {
	t.Skip("SKIPPED FalloAnteriorElegirNuevoLiderTest3")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	time.Sleep(1500 * time.Millisecond)

	fmt.Printf("Lider inicial\n")
	cfg.pruebaUnLider(3)

	cfg.t.Log("Vamos a parar al lider", cfg.idLider)
	// Desconectar lider
	cfg.desconectarNodo("lider") // desconectar el lider

	fmt.Printf("Comprobar nuevo lider\n")
	cfg.pruebaUnLider(3)

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses() //parametros

	fmt.Println(".............", t.Name(), "Superado")
}

// 3 operaciones comprometidas con situacion estable y sin fallos - 3 NODOS RAFT
func (cfg *configDespliegue) tresOperacionesComprometidasEstable(t *testing.T) {
	//t.Skip("SKIPPED tresOperacionesComprometidasEstable")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	time.Sleep(1500 * time.Millisecond)

	cfg.t.Log("Lider inicial")
	cfg.pruebaUnLider(3)

	cfg.t.Log("Vamos a comprometer 3 entradas")
	cfg.comprometerEntrada(0, "escribir", "variable1", "1", "1")
	cfg.comprometerEntrada(1, "escribir", "variable2", "2", "2")
	cfg.comprometerEntrada(2, "leer", "variable1", "", "1")

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses() //parametros

	fmt.Println(".............", t.Name(), "Superado")
}

// Se consigue acuerdo a pesar de desconexiones de seguidor -- 3 NODOS RAFT
func (cfg *configDespliegue) AcuerdoApesarDeSeguidor(t *testing.T) {
	//t.Skip("SKIPPED AcuerdoApesarDeSeguidor")


	cfg.startDistributedProcesses()

	time.Sleep(1500 * time.Millisecond)

	cfg.t.Log("Lider inicial")
	cfg.pruebaUnLider(3)


	cfg.t.Log("Vamos a parar un nodo que no sea el lider")
	cfg.desconectarNodo("no lider")

	cfg.t.Log("Vamos a comprometer 3 entradas")
	cfg.comprometerEntrada(0, "escribir", "variable1", "1", "1")
	cfg.comprometerEntrada(1, "escribir", "variable2", "2", "2")
	cfg.comprometerEntrada(2, "leer", "variable1", "", "1")

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses() //parametros

	fmt.Println(".............", t.Name(), "Superado")
}

// NO se consigue acuerdo al desconectarse mayoría de seguidores -- 3 NODOS RAFT
func (cfg *configDespliegue) SinAcuerdoPorFallos(t *testing.T) {
	//t.Skip("SKIPPED SinAcuerdoPorFallos")
	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	time.Sleep(1500 * time.Millisecond)

	cfg.t.Log("Lider inicial")
	cfg.pruebaUnLider(3)


	cfg.t.Log("Vamos a parar un nodo que no sea el lider")
	cfg.desconectarNodo("no lider todos")

	cfg.t.Log("Vamos a comprometer 1 entrada")
	cfg.noComprometerEntrada(0, "escribir", "variable1", "1", "1")

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses() //parametros

	fmt.Println(".............", t.Name(), "Superado")

}

// Se somete 5 operaciones de forma concurrente -- 3 NODOS RAFT
func (cfg *configDespliegue) SometerConcurrentementeOperaciones(t *testing.T) {
//	t.Skip("SKIPPED SometerConcurrentementeOperaciones")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	time.Sleep(1500 * time.Millisecond)

	cfg.t.Log("Lider inicial")
	cfg.pruebaUnLider(3)

	cfg.t.Log("Vamos a comprometer 5 entradas concurrentemente")
	cfg.comprometerEntradasConcurrentemente()

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses() //parametros

	fmt.Println(".............", t.Name(), "Superado")
}

func (cfg *configDespliegue) desconectarNodo(quien string) {
	var reply raft.Vacio

	if (quien == "lider") {
		for i, ep := range cfg.nodosRaft {
			if i == cfg.idLider {
				err := ep.CallTimeout("NodoRaft.ParaNodo",
					raft.Vacio{}, &reply, 20*time.Millisecond)
				check.CheckError(err, "Error en la llamada RPC ParaNodo")
				cfg.conectados[i] = false
			}
		}

		cfg.t.Log("Parado el lider", cfg.idLider)
		cfg.idLider = -1
	} else if (quien == "no lider") {
		eliminado := false
		nodo := -1
		for i, ep := range cfg.nodosRaft {
			if i != cfg.idLider && !eliminado {
				eliminado = true
				nodo = i
				err := ep.CallTimeout("NodoRaft.ParaNodo",
                                        raft.Vacio{}, &reply, 20*time.Millisecond)
                                check.CheckError(err, "Error en la llamada RPC ParaNodo")
                                cfg.conectados[i] = false
				cfg.t.Log("Parado el nodo", nodo)
			}
		}

	} else if (quien == "no lider todos") {
		nodo := -1
		for i, ep := range cfg.nodosRaft {
			if i != cfg.idLider {
				nodo = i
				err := ep.CallTimeout("NodoRaft.ParaNodo",
                                        raft.Vacio{}, &reply, 20*time.Millisecond)
                                check.CheckError(err, "Error en la llamada RPC ParaNodo")
                                cfg.conectados[i] = false
				cfg.t.Log("Parado el nodo", nodo)
			}
		}
	}

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
				cfg.t.Fatalf("mandato %d tiene %d (>1) lideres",
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
	cfg.t.Fatalf("un lider esperado, ninguno obtenido")

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
	cfg.t.Log("Before start following distributed processes: ", cfg.nodosRaft)

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

	cfg.t.Log("Estado replica 0: ", idNodo, mandato, esLider, idLider, "\n")

	if idNodo != idNodoDeseado || mandato != mandatoDeseado ||
		esLider != esLiderDeseado || idLider != IdLiderDeseado {
		cfg.t.Fatalf("Estado incorrecto en replica %d en subtest %s",
			idNodoDeseado, cfg.t.Name())
	}

}

// Compromete una entrada
func (cfg *configDespliegue) comprometerEntrada(indiceDeseado int,
	operacion string, clave string, valor string, resultadoDeseado string) {

	var reply raft.ResultadoRemoto
	op := raft.TipoOperacion{
		Operacion: operacion,
		Clave:     clave,
		Valor:     valor,
	}
	err := cfg.nodosRaft[cfg.idLider].CallTimeout("NodoRaft.SometerOperacionRaft", &op, &reply, 4000*time.Millisecond)
	check.CheckError(err, "Error en llamada SometerOperacionRaft")

	if indiceDeseado != reply.IndiceRegistro ||
		resultadoDeseado != reply.ValorADevolver {
		cfg.t.Fatalf("No se ha sometido la operación con índice %d y con resultado %s en subtest %s, obtenido el índice %d y el resultado %s",
			indiceDeseado, resultadoDeseado, cfg.t.Name(), reply.IndiceRegistro, reply.ValorADevolver)
	} else {
		cfg.t.Log("Entrada con índice", indiceDeseado, "sometida correctamente")
	}
}

// Intenta comprometer una entrada y comprueba que no se haya comprometido
func (cfg *configDespliegue) noComprometerEntrada(indiceDeseado int,
	operacion string, clave string, valor string, resultadoDeseado string) {

	var reply raft.ResultadoRemoto
	op := raft.TipoOperacion{
		Operacion: operacion,
		Clave:     clave,
		Valor:     valor,
	}
	err := cfg.nodosRaft[cfg.idLider].CallTimeout("NodoRaft.SometerOperacionRaft", &op, &reply, 4000*time.Millisecond)
	if err == nil {
		cfg.t.Fatalf("Se ha comprometido la entrada")
	} else {
		cfg.t.Log("No se ha comprometido la entrada")
	}
}


// Compromete una entrada
func (cfg *configDespliegue) comprometerEntradasConcurrentemente() {

	var reply raft.ResultadoRemoto

	for i:=0; i<5; i++ {
		clave := "variable" + strconv.Itoa(i)
		valor := strconv.Itoa(i)
		op := raft.TipoOperacion {
			Operacion: "escribir",
			Clave: clave,
			Valor: valor,
		}
		go cfg.nodosRaft[cfg.idLider].CallTimeout("NodoRaft.SometerOperacionRaft", &op, &reply, 4000*time.Millisecond)
	}

	time.Sleep(8000 * time.Millisecond)
}
