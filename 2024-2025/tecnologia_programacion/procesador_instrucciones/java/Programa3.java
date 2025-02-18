
public class Programa3 extends Programa
{
    Programa3()
    {
        instr.add(new Push(1));
        instr.add(new Read());        
        instr.add(new Swapp());
        instr.add(new Over());
        instr.add(new Mul());
        instr.add(new Swapp());
        instr.add(new Push(-1));
        instr.add(new Add());
        instr.add(new Dup());
        instr.add(new Push(-2));
        instr.add(new Add());
        instr.add(new Jumpif(2));
        instr.add(new Swapp());
        instr.add(new Write());
    }
}