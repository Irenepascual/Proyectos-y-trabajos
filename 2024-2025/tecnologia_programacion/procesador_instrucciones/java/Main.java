
public class Main
{
    public static void main(String[] args)
    {
        // PRUEBA PROGRAMA 1 - SUMA DE DOS NÚMEROS
        System.out.println("Primer programa: SUMA DE DOS NUMEROS");
        Programa1 p1 = new Programa1();
        System.out.println("Programa:");
        p1.listar();
        System.out.println();
        System.out.println("Ejecución:");
        p1.ejecutar();
        System.out.println();

        // PRUEBA PROGRAMA 2 - CUENTA ATRÁS
        System.out.println("Segundo programa: CUENTA ATRAS");
        Programa2 p2 = new Programa2();
        System.out.println("Programa:");
        p2.listar();
        System.out.println();
        System.out.println("Ejecución");
        p2.ejecutar();
        System.out.println();

        // PRUEBA PROGRAMA 3 - FACTORIAL
        System.out.println("Tercer programa: FACTORIAL");
        Programa3 p3 = new Programa3();
        System.out.println("Programa:");
        p3.listar();
        System.out.println();
        System.out.println("Ejecución");
        p3.ejecutar();
    }
}