// Escribir vuestro código de funcionalidad Raft en este fichero
//

package raft

//
// API
// ===
// Este es el API que vuestra implementación debe exportar
//
// nodoRaft = NuevoNodo(...)
//   Crear un nuevo servidor del grupo de elección.
//
// nodoRaft.Para()
//   Solicitar la parado de un servidor
//
// nodo.ObtenerEstado() (yo, mandato, esLider)
//   Solicitar a un nodo de elección por "yo", su mandato en curso,
//   y si piensa que es el msmo el lider
//
// nodoRaft.SometerOperacion(operacion interface()) (indice, mandato, esLider)

// type AplicaOperacion

import (
	"fmt"
	"io"
	"log"
	"os"

	//"crypto/rand"
	"math/rand"
	"sync"
	"time"

	//"net/rpc"

	"raft/internal/comun/rpctimeout"
)

const (
	// Constante para fijar valor entero no inicializado
	IntNOINICIALIZADO = -1

	//  false deshabilita por completo los logs de depuracion
	// Aseguraros de poner kEnableDebugLogs a false antes de la entrega
	kEnableDebugLogs = true

	// Poner a true para logear a stdout en lugar de a fichero
	kLogToStdout = true

	// Cambiar esto para salida de logs en un directorio diferente
	kLogOutputDir = "./logs_raft/"
)

type TipoOperacion struct {
	Operacion string // las operaciones posibles son "leer" y "escribir"
	Clave     string
	Valor     string // en el caso de la lectura, valor = ""
}

type AplicaOperacion struct {
	Indice    int
	Operacion TipoOperacion
}

type TipoEstadoNodo string

const (
	Lider     TipoEstadoNodo = "Lider"
	Candidato TipoEstadoNodo = "Candidato"
	Seguidor  TipoEstadoNodo = "Seguidor"
)

type TipoEstadoVital string

const (
	Vivo  TipoEstadoVital = "Vivo"
	Caido TipoEstadoVital = "Caido"
)

type TipoReg struct {
	Indice    int
	Mandato   int
	Operacion TipoOperacion
}

// Tipo de dato Go que representa un solo nodo (réplica) de raft
type NodoRaft struct {
	Mux 			  sync.Mutex // Mutex para proteger acceso a estado compartido

	// Host:Port de todos los nodos (réplicas) Raft, en mismo orden
	Nodos   		  []rpctimeout.HostPort
	Yo      		  int // índice de este nodo en campo array "nodos"
	IdLider 		  int
	// Utilización opcional de este logger para depuración
	// Cada nodo Raft tiene su propio registro de trazas (logs)
	Logger 			  *log.Logger

	MandatoActual 		  int
	VotadoA       		  int
	EstadoActual  		  TipoEstadoNodo
	EstadoVital   		  TipoEstadoVital

	Reg       		  []TipoReg
	IndiceReg 		  int

	// NUEVO
	UltimoAplicado    	  int   // índice de al última entrada aplicada
	IndiceComprometido 	  int   // índice de la última entrada comprometida
	SigIndice         	  []int // para cada nodo, índice de la siguiente entrada a enviar
	MatchIndice       	  []int // para cada nodo, índice de la última entrada replicada
	//

	CanalAplicarOperacion     chan AplicaOperacion
	CanalVotos                chan bool
	CanalCambioEstadoActual   chan bool
	CanalLatidos              chan bool

	// NUEVO
	ConfirmacionesCompromiso  int			// numero de nodos con la entrada comprometida
	Almacen			  map[string]string	// almacen de operaciones comprometidas
	CanalOperacionAplicada	  chan bool		// canal para avisar que la operación ya ha sido comprometida
	//
}

// Creacion de un nuevo nodo de eleccion
//
// Tabla de <Direccion IP:puerto> de cada nodo incluido a si mismo.
//
// <Direccion IP:puerto> de este nodo esta en nodos[yo]
//
// Todos los arrays nodos[] de los nodos tienen el mismo orden

// canalAplicar es un canal donde, en la practica 5, se recogerán las
// operaciones a aplicar a la máquina de estados. Se puede asumir que
// este canal se consumira de forma continúa.
//
// NuevoNodo() debe devolver resultado rápido, por lo que se deberían
// poner en marcha Gorutinas para trabajos de larga duracion
func NuevoNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) *NodoRaft {

	nr := &NodoRaft{}
	nr.Nodos = nodos
	nr.Yo = yo
	nr.IdLider = -1

	if kEnableDebugLogs {
		nombreNodo := nodos[yo].Host() + "_" + nodos[yo].Port()

		if kLogToStdout {
			nr.Logger = log.New(os.Stdout, nombreNodo+" -->> ",
				log.Lmicroseconds|log.Lshortfile)
		} else {
			err := os.MkdirAll(kLogOutputDir, os.ModePerm)
			if err != nil {
				fmt.Println("raft.go: antes de panic")
				panic(err.Error())
			}
			logOutputFile, err := os.OpenFile(
				fmt.Sprintf("%s/%s.txt", kLogOutputDir, nombreNodo),
				os.O_RDWR|os.O_CREATE|os.O_TRUNC,
				0755)
			if err != nil {
				fmt.Println("raft.go: antes de panic")
				panic(err.Error())
			}
			nr.Logger = log.New(logOutputFile,
				nombreNodo+" -> ", log.Lmicroseconds|log.Lshortfile)
		}
		nr.Logger.Println("logger initialized")
	} else {
		nr.Logger = log.New(io.Discard, "", 0)
	}

	nr.EstadoActual = Seguidor
	nr.EstadoVital = Vivo
	nr.MandatoActual = 0
	nr.VotadoA = -1

	nr.Reg = nil
	nr.IndiceReg = -1

	// NUEVO
	nr.IndiceComprometido = -1
	nr.UltimoAplicado = -1
	nr.SigIndice = make([]int, len(nr.Nodos))
	nr.MatchIndice = make([]int, len(nr.Nodos))
	for i := 0; i < len(nr.Nodos); i++ {
		nr.SigIndice[i] = 0    // la siguiente entrada a comprometer en el nodo i es la 0
		nr.MatchIndice[i] = -1 // ninguna entrada aplicada en el nodo i
	}
	//

	nr.CanalAplicarOperacion = canalAplicarOperacion
	nr.CanalLatidos = make(chan bool)
	nr.CanalCambioEstadoActual = make(chan bool)
	nr.CanalVotos = make(chan bool)

	// NUEVO
	nr.ConfirmacionesCompromiso = 1
	nr.Almacen = make(map[string]string)
	nr.CanalOperacionAplicada = make(chan bool)
	//

	go nr.maquinaEstados()

	go nr.aplicarOperacionAMaquinaEstados()

	return nr
}

// Metodo Para() utilizado cuando no se necesita mas al nodo
//
// Quizas interesante desactivar la salida de depuracion
// de este nodo
func (nr *NodoRaft) para() {
	nr.EstadoVital = Caido
	go func() { time.Sleep(5 * time.Millisecond); /*os.Exit(0)*/ }()
}

// Devuelve "yo", mandato en curso y si este nodo cree ser lider
//
// Primer valor devuelto es el indice de este  nodo Raft el el conjunto de nodos
// la operacion si consigue comprometerse.
// El segundo valor es el mandato en curso
// El tercer valor es true si el nodo cree ser el lider
// Cuarto valor es el lider, es el indice del líder si no es él
func (nr *NodoRaft) obtenerEstado() (int, int, bool, int) {
	var yo int = nr.Yo
	var mandato int = nr.MandatoActual
	var esLider bool = (nr.IdLider == nr.Yo)
	var idLider int = nr.IdLider
	nr.Logger.Println("raft.go: obtenerEstado(): yo:", yo, "mandato:",
		mandato, "mandatoActual:", nr.MandatoActual,
		"esLider:", esLider, "idLider:", idLider)
	return yo, mandato, esLider, idLider
}

// El servicio que utilice Raft (base de datos clave/valor, por ejemplo)
// Quiere buscar un acuerdo de posicion en registro para siguiente operacion
// solicitada por cliente.

// Si el nodo no es el lider, devolver falso
// Sino, comenzar la operacion de consenso sobre la operacion y devolver en
// cuanto se consiga
//
// No hay garantía que esta operación consiga comprometerse en una entrada
// de registro, dado que el lider puede fallar y la entrada ser reemplazada
// en el futuro.
// Resultado de este método :
// - Primer valor devuelto es el indice del registro donde se va a colocar
// - la operacion si consigue comprometerse.
// - El segundo valor es el mandato en curso
// - El tercer valor es true si el nodo cree ser el lider
// - Cuarto valor es el lider, es el indice del líder si no es él
// - Quinto valor es el resultado de aplicar esta operación en máquina de estados
func (nr *NodoRaft) someterOperacion(operacion TipoOperacion) (int, int,
	bool, int, string) {
	// 1. No soy el lider
	if nr.EstadoActual != Lider {
		nr.Logger.Println("error: no puedo someter la operación porque no soy el líder")
		return -1, nr.MandatoActual, false, nr.IdLider,
			"error: no puedo someter la operacion porque no soy el líder"
	}

	// 2. Sí soy el lider
	indice := nr.IndiceReg+1
	mandato := nr.MandatoActual
	esLider := true
	idLider := nr.Yo

	nr.Mux.Lock()
	nr.IndiceReg++
	nr.Reg = append(nr.Reg, TipoReg{indice, mandato, operacion})
	nr.Mux.Unlock()

	nr.Logger.Println("mi Reg:", nr.Reg, "indiceReg:", nr.IndiceReg)

	// Esperamos a que se haya aplicado la operación a la máquina de estados
	<-nr.CanalOperacionAplicada

	// Devolvemos el valor fijado por el almacen
	valorADevolver := nr.Almacen[operacion.Clave]

	return indice, mandato, esLider, idLider, valorADevolver
}

// -----------------------------------------------------------------------
// LLAMADAS RPC al API
//
// Si no tenemos argumentos o respuesta estructura vacia (tamaño cero)
type Vacio struct{}

func (nr *NodoRaft) ParaNodo(args Vacio, reply *Vacio) error {
	nr.Logger.Println("HE CAIDO")
	defer nr.para()
	return nil
}

type EstadoParcial struct {
	Mandato int
	EsLider bool
	IdLider int
}

type EstadoRemoto struct {
	IdNodo int
	EstadoParcial
}

func (nr *NodoRaft) ObtenerEstadoNodo(args Vacio, reply *EstadoRemoto) error {
	reply.IdNodo, reply.Mandato, reply.EsLider, reply.IdLider = nr.obtenerEstado()
	return nil
}

type ResultadoRemoto struct {
	ValorADevolver string
	IndiceRegistro int
	EstadoParcial
}

func (nr *NodoRaft) SometerOperacionRaft(operacion TipoOperacion,
	reply *ResultadoRemoto) error {
	nr.Logger.Println("Dentro de SometerOperacionRaft()")
	reply.IndiceRegistro, reply.Mandato, reply.EsLider,
		reply.IdLider, reply.ValorADevolver =
		nr.someterOperacion(operacion)
	return nil
}

// -----------------------------------------------------------------------
// LLAMADAS RPC protocolo RAFT
//
// Structura de ejemplo de argumentos de RPC PedirVoto.
//
// Recordar
// -----------
// Nombres de campos deben comenzar con letra mayuscula !
type ArgsPeticionVoto struct {
	Mandato     int
	IdCandidato int
	// NUEVO
	IndiceUltReg  int // índice de la última entrada del Reg del candidato
	MandatoUltReg int // mandato de la última entrada del Reg del candidato
	//
}

// Structura de ejemplo de respuesta de RPC PedirVoto,
//
// Recordar
// -----------
// Nombres de campos deben comenzar con letra mayuscula !
type RespuestaPeticionVoto struct {
	Mandato       int
	VotoConcedido bool
}

// Metodo para RPC PedirVoto
func (nr *NodoRaft) PedirVoto(peticion *ArgsPeticionVoto, reply *RespuestaPeticionVoto) error {
	nr.Mux.Lock()
	defer nr.Mux.Unlock()

	nr.Logger.Println("recibido PedirVoto de nodo", peticion.IdCandidato, "en el mandato", peticion.Mandato)
	nr.Logger.Println("nosotros tenemos mandato", nr.MandatoActual)

	// 1. Si nuestro mandato > su mandato => negar voto
	if nr.MandatoActual > peticion.Mandato {
		nr.Logger.Println("error: rechazado el voto al candidato",
			peticion.IdCandidato, "porque tiene mandato desactualizado")

		reply.VotoConcedido = false
		reply.Mandato = nr.MandatoActual
		return fmt.Errorf("error: rechazado voto al candidato",
			peticion.IdCandidato, "porque tiene mandato desactualizado")

	}

	// 2. Si nuestro mandato < su mandato => conceder voto y actualizarnos
	if nr.MandatoActual < peticion.Mandato {
		nr.Logger.Println("enviado el voto al candidato",
			peticion.IdCandidato, "porque tiene un mandato superior al nuestro")

		nr.MandatoActual = peticion.Mandato
		nr.VotadoA = peticion.IdCandidato
		if nr.EstadoActual != Seguidor {
			nr.EstadoActual = Seguidor
			nr.CanalCambioEstadoActual <- true
		}

		reply.VotoConcedido = true
		reply.Mandato = nr.MandatoActual
		return nil
	}

	// 3. Si nuestro mandato == su mandato
	if nr.MandatoActual == peticion.Mandato {
		if nr.VotadoA == peticion.IdCandidato {
			nr.Logger.Println("enviado el voto al candidato",
				peticion.IdCandidato, "porque ya le habiamos votado antes")

			nr.MandatoActual = peticion.Mandato
			if nr.EstadoActual != Seguidor {
				nr.EstadoActual = Seguidor
				nr.CanalCambioEstadoActual <- true
			}

			reply.VotoConcedido = true
			reply.Mandato = nr.MandatoActual
			return nil
		} else if nr.VotadoA == -1 {
			nr.Logger.Println("error: rechazado el voto al candidato, elecciones cruzadas")

			reply.VotoConcedido = false
			reply.Mandato = nr.MandatoActual
			return fmt.Errorf("error: elecciones cruzadas")
		} else if nr.VotadoA != peticion.IdCandidato {
			nr.Logger.Println("error: rechazado el voto al candidato",
				peticion.IdCandidato, "porque ya habiamos votado al", nr.VotadoA)

			reply.VotoConcedido = false
			reply.Mandato = nr.MandatoActual
			return fmt.Errorf("error: ya habiamos votado a otro candidato")
		}
	}

	return nil
}

type ArgAppendEntries struct {
	Mandato  int
	IdLider  int
	Entradas TipoReg
	// NUEVO
	IndiceAntReg     int // indice de la ultima posicion del Reg
	MandatoAntReg    int // mandato de la última posición del Reg
	IndiceCompromiso int // indice de la posicion de registro donde tiene que comprometer la entrada
	//
}

type Results struct {
	Mandato int
	Exito   bool
}

// Metodo de tratamiento de llamadas RPC AppendEntries
func (nr *NodoRaft) AppendEntries(args *ArgAppendEntries,
	results *Results) error {
	// 1. Hemos recibido un latido del líder
	if args.Entradas == (TipoReg{}) {
		nr.Logger.Println("AppendEntries1: recibido un latido")
		// 1.1. Voy más adelantado que el líder
		if args.Mandato < nr.MandatoActual {
			nr.Logger.Println("AppendEntries2: nosotros vamos más adelantados",
				nr.MandatoActual, "que el", args.Mandato)
			results.Mandato = nr.MandatoActual
			results.Exito = false

		// 1.2. Voy más retrasado que el líder
		} else if args.Mandato > nr.MandatoActual {
			nr.Logger.Println("AppendEntries2: nosotros vamos retrasados", nr.MandatoActual, "que el", args.Mandato, "=> nos actualizamos")
			nr.IdLider = args.IdLider
			nr.MandatoActual = args.Mandato

			results.Mandato = nr.MandatoActual
			results.Exito = true

			if nr.EstadoActual != Seguidor {
				nr.EstadoActual = Seguidor
				nr.CanalCambioEstadoActual <- true
			} else {
				nr.CanalLatidos <- true
			}

		// 1.3. Voy igual que el líder
		} else { /*args.Mandato == nr.MandatoActual*/
			nr.Logger.Println("AppendEntries2: recibido un latido en mandato",
				args.Mandato, "del lider", args.IdLider)
			nr.IdLider = args.IdLider

			results.Mandato = nr.MandatoActual
			results.Exito = true

			nr.CanalLatidos <- true

		}

	// 2. Hemos recibido la solicitud de una entrada a aplicar
	} else {
		nr.Logger.Println("AppendEntries1: recibida la ENTRADA:",
			"índice:", args.Entradas.Indice,
			"mandato:", args.Entradas.Mandato,
			"operación:", args.Entradas.Operacion.Operacion,
			"clave:", args.Entradas.Operacion.Clave,
			"valor:", args.Entradas.Operacion.Valor)

		// 2.1. Voy más adelantado que el líder
		if args.Mandato < nr.MandatoActual {
			nr.Logger.Println("AppendEntries2: el líder está desactualizado")
			results.Mandato = nr.MandatoActual
			results.Exito = false

		// 2.2. Voy más retrasado que el líder
		} else if args.Mandato > nr.MandatoActual {
			nr.Logger.Println("AppendEntries2: nosotros estamos desactualizados")
			nr.IdLider = args.IdLider
			nr.MandatoActual = args.Mandato

			results.Mandato = nr.MandatoActual
			results.Exito = true

			if nr.EstadoActual != Seguidor {
				nr.EstadoActual = Seguidor
				nr.CanalCambioEstadoActual <- true
			} else {
				nr.CanalLatidos <- true
			}

		// 2.3. Voy igual que el líder
		} else { /*args.Mandato == nr.MandatoActual*/
			nr.Logger.Println("AppendEntries2: tenemos mismo mandato")
			nr.IdLider = args.IdLider

			results.Mandato = nr.MandatoActual

			if nr.IndiceReg == -1 {
				// no tenemos ninguna entrada en el Reg
				nr.IndiceReg++
				nr.Reg = append(nr.Reg, args.Entradas)
				results.Exito = true
				nr.Logger.Println("AppendEntries3: operacion añadida al Reg:", nr.Reg, "indiceReg:", nr.IndiceReg)
			} else if args.IndiceAntReg < nr.IndiceReg || nr.Reg[args.IndiceAntReg].Mandato != args.MandatoAntReg {
				results.Exito = false
				nr.Logger.Println("AppendEntries3: no añadida la operacion (reg inconsistente)")
			} else {
				nr.Reg = nr.Reg[0 : args.IndiceAntReg+1]
				nr.IndiceReg++
				nr.Reg = append(nr.Reg, args.Entradas)
				results.Exito = true
				nr.Logger.Println("AppendEntries3: operacion añadida al Reg:", nr.Reg, "indiceReg:", nr.IndiceReg)
			}

			nr.CanalLatidos <- true
		}
	}
	return nil
}

// ------------------------------------------------------------------------
// ----- METODOS/FUNCIONES desde nodo Raft, como cliente, a otro nodo Raft
// ------------------------------------------------------------------------

// Ejemplo de código enviarPeticionVoto
//
// nodo int -- indice del servidor destino en nr.nodos[]
//
// args *RequestVoteArgs -- argumentos par la llamada RPC
//
// reply *RequestVoteReply -- respuesta RPC
//
// Los tipos de argumentos y respuesta pasados a CallTimeout deben ser
// los mismos que los argumentos declarados en el metodo de tratamiento
// de la llamada (incluido si son punteros)
//
// Si en la llamada RPC, la respuesta llega en un intervalo de tiempo,
// la funcion devuelve true, sino devuelve false
//
// la llamada RPC deberia tener un timeout adecuado.
//
// Un resultado falso podria ser causado por una replica caida,
// un servidor vivo que no es alcanzable (por problemas de red ?),
// una petición perdida, o una respuesta perdida
//
// Para problemas con funcionamiento de RPC, comprobar que la primera letra
// del nombre de todo los campos de la estructura (y sus subestructuras)
// pasadas como parametros en las llamadas RPC es una mayuscula,
// Y que la estructura de recuperacion de resultado sea un puntero a estructura
// y no la estructura misma.
func (nr *NodoRaft) enviarPeticionVoto(nodo int, args *ArgsPeticionVoto,
	reply *RespuestaPeticionVoto) bool {

	err := nr.Nodos[nodo].CallTimeout("NodoRaft.PedirVoto", args,
		reply, 20*time.Millisecond)
	if err != nil {
		nr.Logger.Println("raft.go: error al enviar PedirVoto al nodo", nodo)
		return false
	} else {
		nr.Logger.Println("raft.go: enviado PedirVoto correctamente al nodo", nodo)
		// 1. Si nos han concedido el voto
		if reply.VotoConcedido {
			nr.Logger.Println("raft.go: recibido de PedirVoto que SI nos han concedido el voto")
			nr.CanalVotos <- true
			// 2. Si no nos han concedido el voto y estamos desactualizados
		} else if reply.Mandato > nr.MandatoActual {
			nr.Logger.Println("raft.go: recibido de PedirVoto que estamos desactualizados", nr.MandatoActual, reply.Mandato)
			nr.MandatoActual = reply.Mandato
			nr.EstadoActual = Seguidor
			nr.CanalCambioEstadoActual <- true
		}
		return true
	}
}

// ------------------------------------------------------------------------
// ----- METODOS/FUNCIONES AUXILIARES
// ------------------------------------------------------------------------

func (nr *NodoRaft) maquinaEstados() {
	for {
		switch nr.EstadoActual {
		case Seguidor:
			nr.maquinaEstadosSeguidor()
		case Candidato:
			nr.maquinaEstadosCandidato()
		case Lider:
			nr.maquinaEstadosLider()
		}
	}
}

func (nr *NodoRaft) maquinaEstadosSeguidor() {
	nr.Logger.Println("Soy SEGUIDOR en el mandato", nr.MandatoActual)

	randomTimeout := time.Duration(5000+rand.Intn(1000)) * time.Millisecond
	timer := time.NewTimer(randomTimeout)
	defer timer.Stop()

	for nr.EstadoActual == Seguidor {
		select {
		case <-timer.C:
			nr.Logger.Println("soy seguidor y se me ha agotado el tiempo => candidato => eleccion")
			nr.IdLider = -1
			nr.EstadoActual = Candidato
		case <-nr.CanalLatidos:
			timer.Reset(randomTimeout)
		}
	}
}

func (nr *NodoRaft) maquinaEstadosCandidato() {
	nr.Logger.Println("Soy CANDIDATO en el mandato", nr.MandatoActual)

	randomTimeout := time.Duration(5000+rand.Intn(1000)) * time.Millisecond
	timer := time.NewTimer(randomTimeout)
	defer timer.Stop()

	nr.MandatoActual++

	nr.iniciarEleccion()

	votos := 1
	mitadNodos := len(nr.Nodos) / 2

	for nr.EstadoActual == Candidato {
		select {
		case <-timer.C:
			nr.Logger.Println("soy candidato y se me ha agotado el tiempo")
			votos = 1
			nr.MandatoActual++
			timer.Reset(randomTimeout)
			nr.iniciarEleccion()
		case <-nr.CanalLatidos:
			nr.Logger.Println("soy candidato y he recibido un latido => seguidor")
			nr.EstadoActual = Seguidor
		case <-nr.CanalVotos:
			votos++
			nr.Logger.Println("raft.go: tenemos", votos, "votos")
			if votos > mitadNodos {
				nr.Logger.Println("raft.go: tenemos mayoria de votos")
				nr.EstadoActual = Lider
			}
		case <-nr.CanalCambioEstadoActual:
			nr.Logger.Println("dejo de ser candidato")
			if (nr.EstadoActual == Lider) {
				for i:=0; i<len(nr.Nodos); i++ {
					if i != nr.Yo {
						nr.SigIndice[i] = nr.IndiceReg+1
						nr.Logger.Println("cambio de CANDIDATO A LIDER: sigIndice[i]:", nr.SigIndice[i])
						nr.MatchIndice[i] = -1
					}
				}
			}
		}
	}
}

func (nr *NodoRaft) maquinaEstadosLider() {
	nr.Logger.Println("Soy LIDER en el mandato", nr.MandatoActual)

	nr.IdLider = nr.Yo

	timer := time.NewTimer(1000 * time.Millisecond)
	defer timer.Stop()

	nr.enviarLatidos()

	for nr.EstadoActual == Lider {
		select {
		case <-timer.C:
			nr.Logger.Println("soy lider y se ha agotado el tiempo de latido")
			timer.Reset(1000 * time.Millisecond)
			nr.enviarLatidos()
		case <-nr.CanalCambioEstadoActual:
			nr.Logger.Println("dejo de ser lider")
		}
	}
}

func (nr *NodoRaft) iniciarEleccion() {
	nr.Logger.Println("raft.go: yo", nr.Yo, "inicio una eleccion en el mandato", nr.MandatoActual)
	nr.VotadoA = nr.Yo

	var reply RespuestaPeticionVoto
	args := ArgsPeticionVoto{
		Mandato:       nr.MandatoActual,
		IdCandidato:   nr.Yo,
		IndiceUltReg:  nr.IndiceReg,
		MandatoUltReg: 0,
	}
	if nr.IndiceReg > -1 {
		args.MandatoUltReg = nr.Reg[nr.IndiceReg].Mandato
	}

	for i := 0; i < len(nr.Nodos); i++ {
		if i != nr.Yo && nr.EstadoVital != Caido {
			go nr.enviarPeticionVoto(i, &args, &reply)

		}
	}
}

func (nr *NodoRaft) enviarLatidos() {
	// Enviar latido o entrada según corresponda
	var results Results
	for i := 0; i < len(nr.Nodos); i++ {
		if i != nr.Yo {
			// 1. Falta alguna entrada enviar
			if nr.SigIndice[i] <= nr.IndiceReg {
				entrada := TipoReg{
					Indice:    nr.SigIndice[i],
					Mandato:   nr.Reg[nr.SigIndice[i]].Mandato,
					Operacion: nr.Reg[nr.SigIndice[i]].Operacion,
				}
				args := ArgAppendEntries{
					Mandato:          nr.MandatoActual,
					IdLider:          nr.Yo,
					Entradas:         entrada,
					IndiceAntReg:     -1,
					MandatoAntReg:    0,
					IndiceCompromiso: nr.IndiceComprometido,
				}
				nr.Logger.Println("ENVIARLATIDOS: vamos a enviar la ENTRADA:", entrada)

				if nr.SigIndice[i] != 0 {
					args.IndiceAntReg = nr.SigIndice[i] - 1
					args.MandatoAntReg = nr.Reg[nr.SigIndice[i]].Mandato
					args.IndiceCompromiso = nr.IndiceComprometido
				}
				go nr.enviar1Latido(i, &args, &results)

			// 2. Enviar solo un latido
			} else {
				nr.Logger.Println("ENVIARLATIDOS: vamos a enviar un latido")
				args := ArgAppendEntries{
					Mandato:          nr.MandatoActual,
					IdLider:          nr.Yo,
					Entradas:         TipoReg{},
					IndiceAntReg:     -1,
					MandatoAntReg:    0,
					IndiceCompromiso: -1,
				}
				go nr.enviar1Latido(i, &args, &results)
			}
		}
	}
}

func (nr *NodoRaft) enviar1Latido(nodo int, args *ArgAppendEntries,
	results *Results) bool {
	err := nr.Nodos[nodo].CallTimeout("NodoRaft.AppendEntries", args,
		results, 20*time.Millisecond)
	if err != nil {
		return false
	}

	nr.Mux.Lock()
	if !results.Exito {
		// Si nos han respondido false porque vamos retrasados
		if results.Mandato > nr.MandatoActual {
			nr.Logger.Println("nos han respondido false porque vamos retrasados",
				"=> pasamos a ser seguidores")
			nr.MandatoActual = results.Mandato
			nr.IdLider = -1
			nr.EstadoActual = Seguidor
			nr.CanalCambioEstadoActual <- true

		// Si nos han respondido false porque no tenemos bien el registro del nodo
		} else if args.Entradas != (TipoReg{}) {
			nr.SigIndice[nodo]--
		}
	} else {
		// Si nos han respondido true al comprometer una entrada
		if args.Entradas != (TipoReg{}) {
			nr.Logger.Println("RECIBIDA RESPUESTA A UNA ENTRADA: args.Entradas:", args.Entradas, "results:", results)
			nr.MatchIndice[nodo]++
			nr.SigIndice[nodo]++


			// Si aun no habiamos comprometido esa entrada => tenemos una nueva confirmacion de Compromiso
			if nr.IndiceComprometido+1 == args.Entradas.Indice {
				nr.ConfirmacionesCompromiso++
				nr.Logger.Println("confirmaciones de compromiso:", nr.ConfirmacionesCompromiso)
				if nr.ConfirmacionesCompromiso > len(nr.Nodos)/2 {
					nr.IndiceComprometido++
					nr.ConfirmacionesCompromiso = 1
					nr.Logger.Println("tenemos MAYORIA de COMPROMISO de la entrada", nr.IndiceComprometido)

					// enviamos la operacion comprometida para ser aplicada
					nr.CanalAplicarOperacion <- AplicaOperacion{args.Entradas.Indice, args.Entradas.Operacion}
				}
			}
		} else {
			nr.Logger.Println("RECIBIDA RESPUESTA A UN LATIDO")

		}
	}
	nr.Mux.Unlock()
	return true
}

func (nr *NodoRaft) aplicarOperacionAMaquinaEstados() {
	for {
		nr.Logger.Println("aplicarOpAlmacen: operacion aplicada a la maquina de estados")
		// Esperamos a que nos llegue una operación ya comprometida
		op := <- nr.CanalAplicarOperacion

		// Si es una operación de escritura, escribimos en el almacén
		if op.Operacion.Operacion == "escribir" {
			nr.Almacen[op.Operacion.Clave] = op.Operacion.Valor
		}

		// Actualizamos el num de operaciones aplicadas a la máquina de estados
		nr.UltimoAplicado++

		// Avisamos a someterOperacion de que la operación ya ha sido aplicada
		nr.CanalOperacionAplicada <- true
	}
}

