import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Jumpif extends Instruccion
{
    Integer l;
    Jumpif(Integer _l)
    {
        l=_l;
    }     


    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        Integer num = _pila.peek();
        _pila.pop();
        if (num >= 0) {
            _pc.set(l);
        }
        else{
            _pc.incrementAndGet();
        }
    }

    @Override
    String miNombre()
    {
        return "jumpif " + l;
    }
}