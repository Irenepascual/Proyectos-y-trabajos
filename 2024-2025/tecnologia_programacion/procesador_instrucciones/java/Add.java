import java.util.Scanner;
import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;


public class Add extends Instruccion
{
    Add()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        _pc.incrementAndGet();
        Integer n1 = _pila.peek();
        _pila.pop();
        Integer n2 = _pila.peek();
        _pila.pop();
        Integer n3 = n1 + n2;
        _pila.push(n3);
    }

    @Override
    String miNombre()
    {
        return "add";
    }
}