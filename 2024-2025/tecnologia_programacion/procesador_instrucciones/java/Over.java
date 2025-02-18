import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Over extends Instruccion
{
    Over()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        Integer n1 = _pila.peek();
        _pila.pop();
        Integer n2 = _pila.peek();
        _pila.push(n1);
        _pila.push(n2);
        _pc.incrementAndGet();
    }

    @Override
    String miNombre()
    {
        return "over";
    }
}