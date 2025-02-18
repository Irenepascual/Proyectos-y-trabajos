import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Mul extends Instruccion
{
    Mul()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        Integer n1 = _pila.peek();
        _pila.pop();
        Integer n2 = _pila.peek();
        _pila.pop();
        _pila.push(n1*n2);
        _pc.incrementAndGet();
    }

    @Override
    String miNombre()
    {
        return "mul";
    }
}
