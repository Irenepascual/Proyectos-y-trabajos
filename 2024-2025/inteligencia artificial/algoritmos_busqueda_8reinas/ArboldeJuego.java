package aima.core.search.adversarial;

import java.util.ArrayList;
import java.util.List;

public class ArboldeJuego<V> {

    private V valor;   			   		//Tipo integer, Double, Float
    private boolean max;			    //nodo MAX o MIN 
    private ArrayList<ArboldeJuego<V>> hijos;
    private boolean visitado=false; 		// Marca de nodo visitado

    public ArboldeJuego(V valor, boolean max) {
    	this.visitado=false;
        this.valor = valor;
        this.max = max;
        hijos = new ArrayList<ArboldeJuego<V>>();
    }

    public ArboldeJuego(V valor,boolean max,List<ArboldeJuego<V>> hijos) {
        this(valor, max);
        for (ArboldeJuego<V> hijo : hijos)
            this.hijos.add(hijo);
    }

    public boolean esTerminal() {
        return hijos.isEmpty();
    }

    public boolean esMax() {
        return max;
    }

    public void agnadeHijo(ArboldeJuego<V> hijo) {
        this.hijos.add(hijo);
    }

    public V getValor() {
        return this.valor;
    }

    public void setValor(V valor) {
        this.valor = valor;
    }

    public ArrayList<ArboldeJuego<V>> getHijos() {
        return hijos;
    }

    public void setHijos(ArrayList<ArboldeJuego<V>> hijos) {
        this.hijos = hijos;
    }
    
    public void setVisitado() {
        this.visitado = true;
    }
    
    public boolean getVisitado() {
        return this.visitado;
    }

    public String toString() {
        return printArbol(0);
      }
    
  
    public void printArbolExplorado() {
        System.out.println(printArbolExplorado(0));
        System.out.println();
      }

      private static final int indentacion = 4;

      private String printArbol(int increment) {
        String s = "";
        String inc = "";
        for (int i = 0; i < increment; ++i) {
          inc = inc + " ";
        }
        if (this.esTerminal()){ 
        		String formato = "%"+increment+"s";
        		s = inc+ String.format(formato,"["+ valor +"]");
    			//s = inc +"["+ valor +"]"; 
        }
        else {
        		String type;
        		if (this.max) type ="-MAX"; 
        		else type="-MIN";
        		s = inc + valor+type; 
        		
        }
        for (ArboldeJuego<V> hijo : hijos) {
        	  s += "\n" + hijo.printArbol(increment + indentacion);
        }
        return s;
      }
      
      private String printArbolExplorado(int incremento) {
    	  String s = "";
          String inc = "";
          for (int i = 0; i < incremento; ++i) {
            inc = inc + " ";
          }
          if (this.esTerminal()){ 
          		String formato = "%"+incremento+"s";
          		String marca="";
          		if (!this.visitado) marca="X";
          		else marca = valor.toString();
          		s = inc+ String.format(formato,"["+ marca +"]");
      			//s = inc +"["+ valor +"]"; 
          }
          else {
          		String tipo;
          		if (this.max) tipo ="-MAX"; 
          		else tipo="-MIN";
          		String marca="";
          		if (!this.visitado) marca="X";
          		else marca = valor.toString();
          		s = inc + valor+tipo; 
          		
          }
          for (ArboldeJuego<V> hijo : hijos) {
          	  s += "\n" + hijo.printArbolExplorado(incremento + indentacion);
          }
          return s;
        }
      
}