import java.util.Scanner;
import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public abstract class Instruccion
{
    Instruccion()
    {}

    abstract void operacion(Stack<Integer> _pila, AtomicInteger _pc);
    abstract String miNombre();
}

