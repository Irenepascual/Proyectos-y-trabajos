import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Swapp extends Instruccion
{
    Swapp()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {    
        Integer num1 = _pila.peek();
        _pila.pop();
        Integer num2 = _pila.peek();
        _pila.pop();
        _pila.push(num1);
        _pila.push(num2);
        _pc.incrementAndGet();
    }

    @Override
    String miNombre()
    {
        return "swap";
    }
}
