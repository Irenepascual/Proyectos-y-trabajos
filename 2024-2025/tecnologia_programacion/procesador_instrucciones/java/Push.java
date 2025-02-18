import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Push extends Instruccion
{
    Integer c;
    Push(Integer _c)
    {
        c=_c;
    }     
    

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        _pila.push(c);
        _pc.incrementAndGet();
    }

    @Override
    String miNombre()
    {
        return "push " + c;
    }
}
