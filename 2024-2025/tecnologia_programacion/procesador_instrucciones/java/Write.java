import java.util.Scanner;
import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Write extends Instruccion
{
    Write()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        _pc.incrementAndGet();
        Integer num = _pila.peek();
        _pila.pop();
        System.out.println(num);
    }

    @Override
    String miNombre()
    {
        return "write";
    }
}