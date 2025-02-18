package aima.core.util.math;

public class BiseccionPrac2 {

    // Tolerancia para detener la bisección cuando estamos cerca de la raíz
    private static final double TOLERANCIA = 1e-6;

    // Método que implementa la bisección para encontrar el valor de b* (factor de ramificación efectivo)
    public static double calcularFactorRamificacionEfectivo(double N, int d, double a, double b) {
        double mid = 0;

        // Realizar la bisección
        while ((b - a) > TOLERANCIA) {
            mid = (a + b) / 2.0;

            double fMid = funcionNodosGenerados(mid, d);  // f(mid)
            double fA = funcionNodosGenerados(a, d);      // f(a)

            // Verificar si hemos encontrado una raíz
            if (Math.abs(fMid - N) < TOLERANCIA) {
                return mid;  // Hemos encontrado el factor de ramificación efectivo
            }

            // Si f(a) y f(mid) tienen signos opuestos, la raíz está entre a y mid
            if ((fA - N) * (fMid - N) < 0) {
                b = mid;
            } else {
                a = mid;
            }
        }

        return mid;  // Devolvemos el valor aproximado de la raíz
    }

    // Función N = b * (b^d - 1) / (b - 1)
    private static double funcionNodosGenerados(double b, int d) {
        if (b == 1.0) {
            return d + 1;  // Si b = 1, simplificamos la fórmula: N = d + 1
        }
        return (b * (Math.pow(b, d) - 1)) / (b - 1);
    }

    // Método principal para probar la bisección
    public static void main(String[] args) {
        double N = 1000;  // Número de nodos generados
        int d = 5;        // Profundidad de la solución
        double a = 1.1;   // Valor inicial del intervalo
        double b = 10.0;  // Valor final del intervalo

        double bStar = calcularFactorRamificacionEfectivo(N, d, a, b);
        System.out.println("El factor de ramificación efectivo (b*) es: " + bStar);
    }
}

