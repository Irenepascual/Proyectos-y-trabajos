package aima.core.search.adversarial;

public class MiniMaxArbolJuego<V extends Comparable<V>> {

    private int nVisitados = 0;

    public V calcularMiniMax(ArboldeJuego<V> nodo) {
        V valorFinal = miniMaxAux(nodo);
        nodo.printArbolExplorado();
        nVisitados--;
        System.out.println("Nodos visitados: " + nVisitados);
        return valorFinal;
    }

    //////////////////auxiliares/////////////////////
    private V miniMaxAux(ArboldeJuego<V> nodo) {
        if (nodo.esTerminal()) {
        	marcarNodoVisitado(nodo);
            return nodo.getValor();  // Si es terminal, devolvemos su valor directamente
        }
        if (nodo.esMax()) {
        	return ejecutarMax(nodo);
        } else {
        	return ejecutarMin(nodo);
        }
    }
    
    private V ejecutarMax(ArboldeJuego<V> nodo) {
        V mejorValorMax = null;
        for (ArboldeJuego<V> hijo : nodo.getHijos()) {
            V valorHijo = miniMaxAux(hijo);               
            if (mejorValorMax == null || valorHijo.compareTo(mejorValorMax) > 0) {
             	mejorValorMax = valorHijo; // Se selecciona el mayor valor de los hijos
            }
        }
        nodo.setValor(mejorValorMax);
        marcarNodoVisitado(nodo);
        return mejorValorMax;
    }
    
    private V ejecutarMin(ArboldeJuego<V> nodo) {
        V mejorValorMin = null;
        for (ArboldeJuego<V> hijo : nodo.getHijos()) {
            V valorHijo = miniMaxAux(hijo);               
            if (mejorValorMin == null || valorHijo.compareTo(mejorValorMin) < 0) {
            	mejorValorMin = valorHijo; // Se selecciona el menor valor de los hijos
            }
        }
        nodo.setValor(mejorValorMin);
        marcarNodoVisitado(nodo);
        return mejorValorMin;
    }
 
    private void marcarNodoVisitado(ArboldeJuego<V> nodo) {
        nVisitados++;
        nodo.setVisitado();
    }
}
