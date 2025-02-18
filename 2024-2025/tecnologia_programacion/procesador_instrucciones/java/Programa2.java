
public class Programa2 extends Programa
{
    Programa2()
    {
        instr.add(new Read());
        instr.add(new Dup());
        instr.add(new Write());
        instr.add(new Push(-1));
        instr.add(new Add());
        instr.add(new Dup());
        instr.add(new Jumpif(1));
    }
}