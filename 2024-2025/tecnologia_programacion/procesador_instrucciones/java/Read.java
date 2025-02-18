import java.util.Scanner;
import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

public class Read extends Instruccion
{
    Read()
    {}

    @Override
    void operacion(Stack<Integer> _pila, AtomicInteger _pc)
    {
        _pc.incrementAndGet();
        Scanner leer = new Scanner(System.in);
        System.out.print("? ");
        // System.out.flush();
        Integer num = leer.nextInt();

        _pila.push(num);

    }  

    @Override
    String miNombre()
    {
        return "read";
    }  
}