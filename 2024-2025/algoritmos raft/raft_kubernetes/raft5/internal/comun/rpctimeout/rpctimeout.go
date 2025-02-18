package rpctimeout

import (
	"fmt"
	"net/rpc"
	"net"
	"strings"
	"time"
)

type HostPort string // Con la forma, "host:puerto", con host mediante DNS o IP

func MakeHostPort(host, port string) HostPort {
	return  HostPort(host + port)
}

func (hp HostPort) Host() string {
	return string(hp[:strings.Index(string(hp), ":")])
}

func (hp HostPort) Port() string {
	return string(hp[strings.Index(string(hp), ":") + 1:])
}

func (hp HostPort) checkServerReady() error {
    // Intentar establecer una conexión TCP con el servidor
    conn, err := net.DialTimeout("tcp", string(hp), 2*time.Second) // Timeout de 2 segundos
    if err != nil {
        // Si hay un error, significa que el servidor no está listo
        fmt.Printf("Servidor no está listo: %v\n", err)
        return err
    }
    conn.Close()
    return nil // Si la conexión es exitosa, el servidor está listo
}

func (hp HostPort) CallTimeout(serviceMethod string, args interface{}, reply interface{}, timeout time.Duration) error {
    // Tiempo total de espera para todas las tentativas
    start := time.Now()

    // Intentos múltiples para conectar
    for {
        // Verificar si el servidor está listo
        err := hp.checkServerReady()
        if err == nil {
            // Si el servidor está listo, hacer la llamada RPC
            client, err := rpc.Dial("tcp", string(hp))
            if err != nil {
                fmt.Printf("Error dialing endpoint: %v\n", err)
                return err  // Devuelve error de conexión TCP
            }
            defer client.Close() // Cerrar la conexión remota TCP

            done := client.Go(serviceMethod, args, reply, make(chan *rpc.Call, 1)).Done

            // Espera el resultado de la llamada RPC o timeout
            select {
            case call := <-done:
                return call.Error
            case <-time.After(timeout):
                return fmt.Errorf("Timeout in CallTimeout with method: %s, args: %v\n", serviceMethod, args)
            }
        }

        // Si el servidor no está listo, esperamos un poco antes de reintentar
        if time.Since(start) > timeout {
            // Si se alcanzó el tiempo total de espera, devolver error
            return fmt.Errorf("Timeout reached while waiting for server to become ready")
        }

        // Espera antes de reintentar
        fmt.Println("Servidor no listo, reintentando en 1 segundo...")
        time.Sleep(250 * time.Millisecond)
    }

//	reply interface{}, timeout time.Duration) error {
//	time.Sleep(3000*time.Millisecond)
//	client, err := rpc.Dial("tcp", string(hp))

//	if err != nil {
//		fmt.Printf("Error dialing endpoint: %v ", err)
//		return err  // Devuelve error de conexion TCP
//	}
//
//	defer client.Close() // AL FINAL, cerrar la conexion remota  tcp
//
//	done := client.Go(serviceMethod, args, reply,
//					make(chan *rpc.Call, 1)).Done
//
//	select {
//		case call := <-done:
//			return call.Error
//		case <-time.After(timeout):
//			return fmt.Errorf("Timeout in CallTimeout with method: %s, args: %v\n",
//						  serviceMethod,
//						  args)
//	}
}


func StringArrayToHostPortArray(stringArray []string) (result []HostPort) {
	for _ , s := range stringArray {
		result = append(result, HostPort(s))
	}

	return
}

// Array de HostPort end points a un solo string CON ESPACIO DE SEPARACION
func HostPortArrayToString(hostPortArray []HostPort) (result string) {
	for _, hostPort := range hostPortArray {
		result = result + " " + string(hostPort)
	}

	return
}
