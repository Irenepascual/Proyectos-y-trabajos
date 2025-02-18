package aima.core.search.adversarial;

public class AlphaBetaArbolJuego {

    private int nVisitados = 0;

    public Double alphaBetaSearch(ArboldeJuego<Double> nodo) {
        nVisitados = 0; 
        Double resultado = ejecutarAlfaBeta(nodo);
        nodo.printArbolExplorado(); 						
        nVisitados--;
        System.out.println("Nodos visitados: " + nVisitados);
        return resultado;
    }

    private Double ejecutarAlfaBeta(ArboldeJuego<Double> nodo) {
        if (nodo.esMax()) {
            return maxValor(nodo, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
        } else {
            return minValor(nodo, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
        }
    }

    private Double maxValor(ArboldeJuego<Double> nodo, Double alfa, Double beta) {
        nVisitados++;  
        nodo.setVisitado();

        if (nodo.esTerminal()) {
            return nodo.getValor(); // Si es terminal, devolvemos su valor directamente
        }

        Double mejorValor = Double.NEGATIVE_INFINITY; // Iniciamos con el valor más bajo posible para MAX

        for (ArboldeJuego<Double> hijo : nodo.getHijos()) {
            Double valorHijo = minValor(hijo, alfa, beta);
            mejorValor = Math.max(mejorValor, valorHijo);

            // Actualizamos alfa solo si es necesario
            alfa = Math.max(alfa, mejorValor);

            // Si alfa es mayor o igual a beta, realizamos poda
            if (alfa >= beta) {
                break;
            }
        }

        nodo.setValor(mejorValor); // Asignamos el valor al nodo padre
        return mejorValor;
    }

    private Double minValor(ArboldeJuego<Double> nodo, Double alfa, Double beta) {
        nVisitados++; 
        nodo.setVisitado();

        if (nodo.esTerminal()) {
            return nodo.getValor(); // Si es terminal, devolvemos su valor directamente
        }

        Double mejorValor = Double.POSITIVE_INFINITY; // Iniciamos con el valor más alto posible para MIN

        for (ArboldeJuego<Double> hijo : nodo.getHijos()) {
            Double valorHijo = maxValor(hijo, alfa, beta);
            mejorValor = Math.min(mejorValor, valorHijo);

            // Actualizamos beta solo si es necesario
            beta = Math.min(beta, mejorValor);

            // Si mejorValor es menor o igual a alfa, realizamos poda
            if (mejorValor <= alfa) {
                break;
            }
        }

        nodo.setValor(mejorValor); // Asignamos el valor al nodo padre
        return mejorValor;
    }
}
