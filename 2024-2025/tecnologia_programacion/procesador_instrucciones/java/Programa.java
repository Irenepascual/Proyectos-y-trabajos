import java.util.Stack;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicInteger;

public abstract class Programa
{
    List<Instruccion> instr;
    AtomicInteger pc;
    Stack<Integer> pila;

    Programa()
    {
        instr = new ArrayList<>();
        pc = new AtomicInteger(0);
        pila = new Stack<>();
    }

    void listar()
    {
        for (Integer i=0; i<instr.size(); i++) {
            System.out.printf("%2d",i);
            System.out.println("   " + instr.get(i).miNombre());
        }
    }
    void ejecutar()
    {
        while (pc.get() < instr.size()) {
            instr.get(pc.get()).operacion(pila,pc);
        }
    }
}