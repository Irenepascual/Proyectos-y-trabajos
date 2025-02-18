
public class Programa1 extends Programa
{
    Programa1()
    {
        instr.add(new Read());
        instr.add(new Read());        
        instr.add(new Add());
        instr.add(new Write());
    }
}