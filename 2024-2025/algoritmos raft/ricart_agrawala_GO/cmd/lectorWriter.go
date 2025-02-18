/*
* AUTOR: Irene Pascual Albericio 871627 y Ariana Porroche Llorén 874055
* FECHA: octubre de 2024
* FICHERO: lectorWriter.go
 */
package cmd

import (
	"os"
)

type LectorWriter struct {
	ImLector bool // true si es lector, false si es escritor
}

func New(itsLector bool) LectorWriter {
	return LectorWriter{itsLector}
}

func Exclude(lw1 LectorWriter, lw2 LectorWriter) bool {
	if lw1.ImLector && lw2.ImLector {
		return false
	} else {
		return true
	}
}

func LeerFichero(fichero string) (string, error) {
	contenido, err := os.ReadFile(fichero)
	if err != nil {
		return "", err
	}

	return string(contenido), nil
}

func EscribirFichero(fichero string, fragmento string) error {
	file, err := os.OpenFile(fichero, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		return err
	}
	defer file.Close()

	_, err = file.WriteString(fragmento + "\n")
	if err != nil {
		return err
	}

	return nil
}
