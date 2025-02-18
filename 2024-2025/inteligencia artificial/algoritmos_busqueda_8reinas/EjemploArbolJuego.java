package aima.core.search.adversarial;

import java.util.ArrayList;

public class EjemploArbolJuego {

    public static void main(String[] args) {
        // CREAR ÁRBOL PARA MINIMAX
    	ArrayList<ArboldeJuego<Double>> sublista1 = new ArrayList<>();
    	sublista1.add(new ArboldeJuego<>(3.0, true));  // MAX, terminal 3.0
    	sublista1.add(new ArboldeJuego<>(12.0, true)); // MAX, terminal 12.0
    	sublista1.add(new ArboldeJuego<>(8.0, true));  // MAX, terminal 8.0

    	ArboldeJuego<Double> subarbol1 = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, false, sublista1);


        ArrayList<ArboldeJuego<Double>> sublista2 = new ArrayList<>();
        sublista2.add(new ArboldeJuego<>(2.0, true)); // MAX, terminal 2
        sublista2.add(new ArboldeJuego<>(4.0, true)); // MAX, terminal 4
        sublista2.add(new ArboldeJuego<>(6.0, true)); // MAX, terminal 6

        ArboldeJuego<Double> subarbol2 = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, false, sublista2);

        ArrayList<ArboldeJuego<Double>> sublista3 = new ArrayList<>();
        sublista3.add(new ArboldeJuego<>(14.0, true)); // MAX, terminal 14
        sublista3.add(new ArboldeJuego<>(5.0, true));  // MAX, terminal 5
        sublista3.add(new ArboldeJuego<>(2.0, true));  // MAX, terminal 2

        ArboldeJuego<Double> subarbol3 = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, false, sublista3);

        //CREAR ÁRBOL RAÍZ Y AÑADIR SUBÁRBOLES
        ArrayList<ArboldeJuego<Double>> arbolPrincipal = new ArrayList<>();
        arbolPrincipal.add(subarbol1);
        arbolPrincipal.add(subarbol2);
        arbolPrincipal.add(subarbol3);

        ArboldeJuego<Double> arbolRaiz = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, true, arbolPrincipal);

        //EJECUTAR MINIMAX
        MiniMaxArbolJuego<Double> minimax = new MiniMaxArbolJuego<>();
        Double valorMinimax = minimax.calcularMiniMax(arbolRaiz);
        arbolRaiz.setValor(valorMinimax);
        System.out.println("Valor con MINIMAX: " + valorMinimax);
        System.out.println("-------------------------------");

        //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////        
        
        //CREAR ÁRBOL PARA PODA ALFA-BETA
     // CREAR ÁRBOL PARA PODA ALFA-BETA
        ArrayList<ArboldeJuego<Double>> sublista4 = new ArrayList<>();
        sublista4.add(new ArboldeJuego<>(3.0, true));  // MAX, terminal 3.0
        sublista4.add(new ArboldeJuego<>(12.0, true)); // MAX, terminal 12.0
        sublista4.add(new ArboldeJuego<>(8.0, true));  // MAX, terminal 8.0

        ArboldeJuego<Double> subarbol4 = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, false, sublista4);

        ArrayList<ArboldeJuego<Double>> sublista5 = new ArrayList<>();
        sublista5.add(new ArboldeJuego<>(2.0, true));  // MAX, terminal 2.0
        sublista5.add(new ArboldeJuego<>(4.0, true));  // MAX, terminal 4.0
        sublista5.add(new ArboldeJuego<>(6.0, true));  // MAX, terminal 6.0

        ArboldeJuego<Double> subarbol5 = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, false, sublista5);

        ArrayList<ArboldeJuego<Double>> sublista6 = new ArrayList<>();
        sublista6.add(new ArboldeJuego<>(14.0, true)); // MAX, terminal 14.0
        sublista6.add(new ArboldeJuego<>(5.0, true));  // MAX, terminal 5.0
        sublista6.add(new ArboldeJuego<>(2.0, true));  // MAX, terminal 2.0

        ArboldeJuego<Double> subarbol6 = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, false, sublista6);


        //CREAR ÁRBOL RAÍZ PARA ALFA-BETA Y AÑADIR NUEVOS SUBÁRBOLES
        ArrayList<ArboldeJuego<Double>> arbolPrincipalAlfaBeta = new ArrayList<>();
        arbolPrincipalAlfaBeta.add(subarbol4);
        arbolPrincipalAlfaBeta.add(subarbol5);
        arbolPrincipalAlfaBeta.add(subarbol6);

        ArboldeJuego<Double> arbolRaizAlfaBeta = new ArboldeJuego<>(Double.NEGATIVE_INFINITY, true, arbolPrincipalAlfaBeta);

        //EJECUTAR ALFA-BETA
        AlphaBetaArbolJuego alfaBeta = new AlphaBetaArbolJuego();
        Double valorAlfaBeta = alfaBeta.alphaBetaSearch(arbolRaizAlfaBeta);
        arbolRaizAlfaBeta.setValor(valorAlfaBeta);
        System.out.println("Valor con poda Alfa Beta: " + valorAlfaBeta);
        System.out.println("-------------------------------");
    }
}
