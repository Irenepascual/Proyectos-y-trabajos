import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Dup extends Instruccion
{
    Dup()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        Integer num = _pila.peek();
        _pila.push(num);
        _pc.incrementAndGet();
    }

    @Override
    String miNombre()
    {
        return "dup";
    }
}
