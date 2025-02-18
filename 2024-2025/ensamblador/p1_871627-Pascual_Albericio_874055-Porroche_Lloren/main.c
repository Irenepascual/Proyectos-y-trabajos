// fuentes Proyecto Hardware 2024
// version 0.1

#include "matriz_3x3.h"

extern uint8_t matriz3x3_operar_ARM(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);
extern uint8_t matriz3x3_multiplicar_ARM(int A[N][N], int B[N][N], int Resultado[N][N]);
extern uint8_t matriz3x3_trasponer_ARM(int resultado2[N][N]);
extern uint8_t matriz3x3_sumar_ARM(int Resultado[N][N], int resultado2[N][N]);
extern uint8_t matriz3x3_operar_THB(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);

    // Inicialización de matrices usando el tipo de datos Matriz3x3
static int Test_A[N][N] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}
	};

static int Test_B[N][N] = {
		{9, 8, 7},
		{6, 5, 4},
		{3, 2, 1}
	};

		
/* *****************************************************************************
 * declaración funciones internas 
 */
		
// funcion que ejecuta las distintas versiones de la implementacion en C, ARM y Thumb y verifica que dan el mismo resultado.
// recibe las matrices con las que operar
// devuelve si todas las versiones coinciden en el numero de terminos_no_cero o no y el resultado de la operación.
uint8_t matrizNxN_verificar(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);

// procedimiento que traspone la matriz mat y actualiza su valor (en C)
void matrizNxN_trasponer_C(int mat[N][N]);

// función que suma la matriz mat y la matriz mat2 y guarda el resultado en mat
// devuelve el numero de terminos_no_cero del resultado de la suma
uint8_t matrizNxN_sumar_C(int mat[N][N], int mat2[N][N]);


/* *****************************************************************************
 * IMPLEMENTACIONES 
 */
		
void matrizNxN_multiplicar_C(int A[N][N], int B[N][N], int Resultado[N][N]){
	// CÓDIGO ESPECÍFICO PARA N=3
	for (int i=0; i<N; i++) {
		for (int j=0; j<N; j++) {
			Resultado[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j];
		}
	}
}


//funcion que calcula Resultado = A*B + transpuesta(C*D) y devuelva el numero de terminos distintos de cero en el Resultado
//ayudandose de funcion matrizNxN_multiplicar_C que calcula A*B de NxN
uint8_t matrizNxN_operar_C(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]){
	uint8_t terminos_no_cero;
	
	matrizNxN_multiplicar_C(A, B, Resultado);
	
	int resultado2[N][N] = {0};
	matrizNxN_multiplicar_C(C, D, resultado2);
	matrizNxN_trasponer_C(resultado2);
	
	terminos_no_cero = matrizNxN_sumar_C(Resultado,resultado2);
	return terminos_no_cero;
}

		
uint8_t matrizNxN_verificar(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]){
	uint8_t terminos_no_cero_C;
	uint8_t terminos_no_cero_ARM_C;
	uint8_t terminos_no_cero_ARM;
	uint8_t terminos_no_cero_THB;
	uint8_t resultado;
	
	terminos_no_cero_C = matrizNxN_operar_C(A, B, C, D, Resultado);
	terminos_no_cero_ARM_C = matriz3x3_operar_ARM_C(A, B, C, D, Resultado);	
	terminos_no_cero_ARM = matriz3x3_operar_ARM(A, B, C, D, Resultado);
	terminos_no_cero_THB = matriz3x3_operar_THB(A, B, C, D, Resultado);
	
	resultado = (terminos_no_cero_C != terminos_no_cero_ARM_C || terminos_no_cero_C != terminos_no_cero_ARM || terminos_no_cero_ARM != terminos_no_cero_ARM_C || terminos_no_cero_C != terminos_no_cero_THB);
																																																			  
	return resultado;
}



void matrizNxN_trasponer_C(int mat[N][N]) {
	for (int i=0; i<N; i++) {
		for (int j=i+1; j<N; j++) {
			int temp = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = temp;
    }
	}
}


uint8_t matrizNxN_sumar_C(int mat[N][N], int mat2[N][N]) {
	uint8_t distintos = 0;
	for (int i=0; i<N; i++) {
		for (int j=0; j<N; j++) {
			mat[i][j] += mat2[i][j];
			if (mat[i][j] != 0) {
				distintos++;
			}
		}
	}
	return distintos;
}


// MAIN 
int main (void) {
	int Resultado_E[N][N];
	int error;

	int Test_C[N][N]  = {
		{1, 0, 2},
		{0, 1, 2},
		{2, 0, 1}
	};

	int Test_D[N][N]  = {
		{2, 1, 0},
		{1, 2, 0},
		{0, 0, 2}
	};
		
	error = matrizNxN_verificar(Test_A, Test_B, Test_C, Test_D, Resultado_E);

	while(1); //no hay S.O., no se retorna
}

/**
 *@}
 **/
