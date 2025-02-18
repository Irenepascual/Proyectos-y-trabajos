		AREA datos, DATA
matAux  SPACE   3*3*4
		
		AREA codigo, CODE
		PRESERVE8 {TRUE}
		EXPORT matriz3x3_operar_ARM_C
		EXPORT matriz3x3_operar_ARM
		EXPORT matriz3x3_operar_THB
		IMPORT matrizNxN_multiplicar_C
	
		ENTRY
		ARM
	
		; r0 = @A
		; r1 = @B
		; r2 = @C
		; r3 = @D
		; (r4 = @Resultado)
matriz3x3_operar_ARM	
		mov	ip,sp
		STMDB sp!,{r4-r7,fp,lr}
		sub fp,ip,#4
		
		LDR r4,[fp,#4]			; r4=@Resultado
		mov	r5,r2				; r5=@C
		mov r6,r3				; r6=@D
		LDR r7,=matAux			; r7=@matAux=@resultado2

		mov r2,r4				; r2=@Resultado
		bl	matriz3x3_multiplicar_ARM

		mov r0,r5				; r0=@C
		mov r1,r6				; r1=@D
		mov r2,r7				; r2=@resultado2
		bl	matriz3x3_multiplicar_ARM
		
		mov r0,r7				; r0=@resultado2
		bl	matriz3x3_trasponer_ARM
		
		mov r0,r4				; r0=@Resultado
		mov r1,r7				; r1=@resultado2
		bl	matriz3x3_sumar_ARM
		
		; terminos_no_cero ya está en r0
		LDMIA sp!,{r4-r7,fp,lr}
		bx lr	


;******************************************************************************************************

		; r0 = @A
		; r1 = @B
		; r2 = @C
		; r3 = @D
		; (r4 = @Resultado)
matriz3x3_operar_ARM_C	
		mov	ip,sp
		STMDB sp!,{r4-r7,fp,lr}
		sub fp,ip,#4
		
		LDR r4,[fp,#4]			; r4=@Resultado
		mov	r5,r2				; r5=@C
		mov r6,r3				; r6=@D
		LDR r7,=matAux			; r7=@matAux=@resultado2
		
		mov	r2,r4				; r2=@Resultado
		bl	matrizNxN_multiplicar_C
		
		mov r0,r5				; r0=@C
		mov r1,r6				; r1=@D
		mov r2,r7				; r2=@resultado2
		bl	matrizNxN_multiplicar_C
		
		mov r0,r7				; r0=@resultado2
		bl	matriz3x3_trasponer_ARM
		
		mov r0,r4				; r0=@Resultado
		mov r1,r7				; r1=@resultado2
		bl	matriz3x3_sumar_ARM
		
		; terminos_no_cero ya está en r0
		LDMIA sp!,{r4-r7,fp,lr}
		bx lr	


;******************************************************************************************************
	
		; función que recibe como parámetros las matrices A, B y Resultado
		; calcula la multiplicación de las matrices A y B y guarda su valor en Resultado
		; r0 = @A
		; r1 = @B
		; r2 = @Resultado
matriz3x3_multiplicar_ARM
		mov	ip,sp
		STMDB sp!,{r4-r7,fp,lr}
		sub fp,ip,#4
		
		mov r3,#2			; r3=N-1=2=i
b1MA	cmp r3,#0
		blt	fin1MA			; salta si i=0
		
		mov r4,#2			; r4=N-1=2=j
b2MA	cmp r4,#0
		blt	fin2MA			; salta si j=0

		; A[i][0]
		mov r5,#3			; r5=N=3
		mul r5,r3,r5		; r5=N*i
		lsl r5,r5,#2		; r5=4*(N*i)
		add r5,r0,r5		; r5=@A+4*(N*i)=@A[i][0]
		LDR  r5,[r5]		; r5=A[i][0]
		; B[0][j]
		mov r6,r4			; r6=j
		lsl r6,r6,#2		; r6=4*j
		add r6,r1,r6		; r6=@B+4*j=@B[0][j]
		LDR	 r6,[r6]		; r6=B[0][j]
		; A[i][0]*B[0][j]
		mov r7,r6			; 
		mul r7,r5,r7		; r7=A[i][0]*B[0][j]
		
		; A[i][1]
		mov r5,#3			; r5=N=3
		mul r5,r3,r5		; r5=N*i
		add r5,r5,#1		; r5=N*i+1
		lsl r5,r5,#2		; r5=4*(N*i+1)
		add r5,r0,r5		; r5=@A+4*(N*i+1)=@A[i][1]
		LDR  r5,[r5]		; r5=A[i][1]	
		; B[1][j]
		mov r6,#3			; r6=N=3
		add r6,r6,r4		; r6=N+j
		lsl r6,r6,#2		; r6=4*(N+j)
		add r6,r1,r6		; r6=@B+4*(N+j)=@B[1][j]
		LDR	 r6,[r6]		; r6=B[1][j]
		; A[i][1]*B[1][j]
		mul r6,r5,r6		; r6=A[i][1]+B[1][j]
		add r7,r7,r6		; r7=A[i][0]*B[0][j] + A[i][1]+B[1][j]
		
		; A[i][2]
		mov r5,#3			; r5=N=3
		mul r5,r3,r5		; r5=N*i
		add r5,r5,#2		; r5=N*i+2
		lsl r5,r5,#2		; r5=4*(N*i+2)
		add r5,r0,r5		; r5=@A+4*(N*i+2)=@A[i][2]
		LDR  r5,[r5]		; r5=A[i][2]	
		; B[2][j]
		mov r6,#3			; r6=N=3
		lsl r6,r6,#1		; r6=N*2
		add r6,r6,r4		; r6=N+j
		lsl r6,r6,#2		; r6=4*(N*2+j)
		add r6,r1,r6		; r6=@B+4*(N*2+j)=@B[2][j]
		LDR	 r6,[r6]		; r6=B[2][j]
		; A[i][2]*B[2][j]
		mul r6,r5,r6		; r6=A[i][2]+B[2][j]
		add r7,r7,r6		; r7=A[i][0]*B[0][j] + A[i][1]+B[1][j] + A[i][2]+B[2][j]
		
		; Resultado[i][j]
		mov r5,#3			; r5=N=3
		mul r5,r3,r5		; r5=N*i
		add r5,r5,r4		; r5=N*i+j
		lsl r5,r5,#2		; r5=4*(N*i+j)
		add r5,r2,r5		; r5=@Resultado+4*(N*i+j)=@Resultado[i][j]
		STR  r7,[r5]		; guardamos A[i][0]*B[0][j] + A[i][1]+B[1][j] + A[i][2]+B[2][j] en @Resultado[i][j]
		
		sub r4,r4,#1
		b	b2MA
		
fin2MA	sub r3,r3,#1
		b	b1MA
		
fin1MA	LDMIA sp!,{r4-r7,fp,lr}
		bx lr	

	
;--------------------------------------------------------------------------------

		; función en ARM que recibe como parámetro la matriz mat
		; traspone dicha matriz y actualiza su valor
		; r0 = @mat
matriz3x3_trasponer_ARM
        mov ip,sp
        STMDB sp!,{r4-r7,fp,lr}
        sub fp,ip,#4
		
		mov r1,#3			; r1=N=3

		mov r2,#0			; r2=0=i
bT1		cmp r2,r1
		bge	finT1			; salta si i>=N
		
		add r3,r2,#1		; r3=j=i+1
bT2		cmp	r3,r1		
		bge	finT2			; salta si j>=N
		
		mla r4,r1,r2,r3		; r4=3*i+j
		add	r4,r0,r4,lsl #2	; r4=@mat+4*(3*i+j)=@mat[i][j]
		
		mla r5,r1,r3,r2
		add r5,r0,r5,lsl #2	; r5=@mat+4*(3*j+i)=@mat[j][i]

		LDR r6,[r4]			; r6=mat[i][j]
		LDR r7,[r5]			; r7=mat[j][i]
		
		STR r7,[r4]			; guardamos mat[j][i] en @mat[i][j]
		STR	r6,[r5]			; guardamos mat[i][j] en @mat[j][i]
		
		add r3,r3,#1		; j++
		b	bT2
		
finT2	add r2,r2,#1		; i++
		b	bT1
		
finT1	LDMIA sp!,{r4-r7,fp,lr}
        bx lr	
		
		
;--------------------------------------------------------------------------------
		

		; función en ARM que recibe como parámetros las matrices matriz1 y matriz2
		; calcula la suma de las dos matrices y guarda el resultado en matriz1
		; devuelve el ´numero de terminos_no_cero del resultado de la suma
		; r0 = @matriz1
		; r1 = @matriz2
matriz3x3_sumar_ARM
		mov ip,sp
		stmdb sp!,{r4-r6,fp,lr}
		sub fp,ip,#4
		
		mov r2,#3			; r2=N=3
		mul r3,r2,r2
		lsl r2,r3,#2		; r2=N*N*4
		mov r3,#0			; r3=0=iterador
		mov r6,#0			; r6=0=terminos_no_cero
		
bS		cmp	r3,r2
		bge	finS			; salta si iterador>=N*N*4
		
		LDR	r4,[r0,r3]		; r4=matriz1[iterador]
		LDR r5,[r1,r3]		; r5=matriz2[iterador]
		add r4,r4,r5		; r4=matriz1[iterador]+matriz2[iterador]
		
		STR r4,[r0,r3]		; guardamos r4 en matriz1[iterador]
		
		cmp r4,#0
		addne r6,r6,#1		; terminos_no_cero++
		
		add r3,r3,#4		; r3=iterador+4 (sig. pos.)
		b	bS

finS 	mov r0,r6			; r0 <- terminos_no_cero
		LDMIA sp!,{r4-r6,fp,lr}
		bx lr
	
;******************************************************************************************************
	
		THUMB
		; función en THUMB que recibe como parámetros las matrices A, B, C, D y Resultado
		; calcula A*B+(C*D)^t
		; r0 = @A
		; r1 = @B
		; r2 = @C
		; r3 = @D
		; r4 = @Resultado
prog_THB 
		PUSH {r4}
		PUSH {r2-r3}
		movs r2,r4				; r2=@Resultado
		bl matriz3x3_multiplicar_THB
		
		;r0=@C; r1=@D; r2=@matAux
		POP	 {r0-r1}			; r0=@C, r1=@D
		LDR r2,=matAux
		bl matriz3x3_multiplicar_THB
		
		;r0=@matAux
		LDR r0,=matAux
		bl matriz3x3_trasponer_THB
		
		;r0=@R; r1=@Resultado
		POP {r0}				; r0=@Resultado
		LDR	r1,=matAux
		bl matriz3x3_sumar_THB

		; retornar a arm
		adr r1, backARM
		bx r1


;--------------------------------------------------------------------------------

	ARM
		; función en ARM que recibe como parámetros las matrices A, B, C, D y Resultado
		; llama a la función prog_THB que calcula en THUMB la operación: A*B+(C*D)^t
		; r0 = @A
		; r1 = @B
		; r2 = @C
		; r3 = @D
		; (r4 = @Resultado)
matriz3x3_operar_THB
		mov	ip,sp
		STMDB sp!,{r4-r5,fp,lr}
		sub fp,ip,#4
		LDR r4,[fp,#4]			; r4=@Resultado
		
		adr r5, prog_THB+1
		bx r5
		NOP
		
backARM	LDMIA sp!,{r4-r5,fp,lr}
		bx lr	


;--------------------------------------------------------------------------------

	THUMB
		; función en THUMB que recibe como parámetros las matrices A, B y Resultado
		; calcula la multiplicación de las matrices A y B y guarda su valor en Resultado
		; r0 = @A
		; r1 = @B
		; r2 = @Resultado
matriz3x3_multiplicar_THB
		PUSH {lr}
		STMDB sp!,{r4-r7}
		movs r3,#2			; r3=N-1=2=i
b1THB	cmp r3,#0
		blt	fin1THB			; salta si i=0
		
		movs r4,#2			; r4=N-1=2=j
b2THB	cmp r4,#0
		blt	fin2THB			; salta si j=0

		; A[i][0]
		movs r5,#3			; r5=N=3
		muls r5,r3,r5		; r5=N*i
		lsls r5,r5,#2		; r5=4*(N*i)
		adds r5,r0,r5		; r5=@A+4*(N*i)=@A[i][0]
		LDR  r5,[r5]		; r5=A[i][0]
		; B[0][j]
		movs r6,r4			; r6=j
		lsls r6,r6,#2		; r6=4*j
		adds r6,r1,r6		; r6=@B+4*j=@B[0][j]
		LDR	 r6,[r6]		; r6=B[0][j]
		; A[i][0]*B[0][j]
		movs r7,r6			; 
		muls r7,r5,r7		; r7=A[i][0]*B[0][j]
		
		; A[i][1]
		movs r5,#3			; r5=N=3
		muls r5,r3,r5		; r5=N*i
		adds r5,r5,#1		; r5=N*i+1
		lsls r5,r5,#2		; r5=4*(N*i+1)
		adds r5,r0,r5		; r5=@A+4*(N*i+1)=@A[i][1]
		LDR  r5,[r5]		; r5=A[i][1]	
		; B[1][j]
		movs r6,#3			; r6=N=3
		adds r6,r6,r4		; r6=N+j
		lsls r6,r6,#2		; r6=4*(N+j)
		adds r6,r1,r6		; r6=@B+4*(N+j)=@B[1][j]
		LDR	 r6,[r6]		; r6=B[1][j]
		; A[i][1]*B[1][j]
		muls r6,r5,r6		; r6=A[i][1]+B[1][j]
		adds r7,r7,r6		; r7=A[i][0]*B[0][j] + A[i][1]+B[1][j]
		
		; A[i][2]
		movs r5,#3			; r5=N=3
		muls r5,r3,r5		; r5=N*i
		adds r5,r5,#2		; r5=N*i+2
		lsls r5,r5,#2		; r5=4*(N*i+2)
		adds r5,r0,r5		; r5=@A+4*(N*i+2)=@A[i][2]
		LDR  r5,[r5]		; r5=A[i][2]	
		; B[2][j]
		movs r6,#3			; r6=N=3
		lsls r6,r6,#1		; r6=N*2
		adds r6,r6,r4		; r6=N+j
		lsls r6,r6,#2		; r6=4*(N*2+j)
		adds r6,r1,r6		; r6=@B+4*(N*2+j)=@B[2][j]
		LDR	 r6,[r6]		; r6=B[2][j]
		; A[i][2]*B[2][j]
		muls r6,r5,r6		; r6=A[i][2]+B[2][j]
		adds r7,r7,r6		; r7=A[i][0]*B[0][j] + A[i][1]+B[1][j] + A[i][2]+B[2][j]
		
		; Resultado[i][j]
		movs r5,#3			; r5=N=3
		muls r5,r3,r5		; r5=N*i
		adds r5,r5,r4		; r5=N*i+j
		lsls r5,r5,#2		; r5=4*(N*i+j)
		adds r5,r2,r5		; r5=@Resultado+4*(N*i+j)=@Resultado[i][j]
		STR  r7,[r5]		; guardamos A[i][0]*B[0][j] + A[i][1]+B[1][j] + A[i][2]+B[2][j] en @Resultado[i][j]
		
		subs r4,r4,#1
		b	b2THB
		
fin2THB	subs r3,r3,#1
		b	b1THB
		
fin1THB	LDMIA sp!,{r4-r7}
		POP {pc}
		
		
;--------------------------------------------------------------------------------

		; función en THUMB que recibe como parámetros las matrices matriz1 y matriz2
		; calcula la suma de las dos matrices y guarda el resultado en matriz1
		; devuelve el ´numero de terminos_no_cero del resultado de la suma
		; r0 = @matriz1
		; r1 = @matriz2
matriz3x3_sumar_THB
		PUSH {lr}
		STMDB sp!,{r4-r7}
		
		movs r2,#3			; r2=N=3
		movs r3,r2
		muls r3,r2,r3
		lsls r2,r3,#2		; r2=N*N*4
		movs r3,#0			; r3=0=iterador
		movs r6,#0			; r6=0=terminos_no_cero
		
bSTHB	cmp	r3,r2
		bge	finSTHB			; salta si iterador>=N*N*4
		
		LDR	r4,[r0,r3]		; r4=matriz1[iterador]
		LDR r5,[r1,r3]		; r5=matriz2[iterador]
		adds r4,r4,r5		; r4=matriz1[iterador]+matriz2[iterador]
		
		STR r4,[r0,r3]		; guardamos r4 en matriz1[iterador]
		
		cmp r4,#0
		bne term_O
				
seguirTHB	adds r3,r3,#4		; r3=iterador+4 (sig. pos.)
		b	bSTHB

term_O	adds r6,r6,#1		; terminos_no_cero++
		b seguirTHB

finSTHB	movs r0,r6			; r0=terminos_no_cero
		LDMIA sp!,{r4-r7}
		POP {pc}


;--------------------------------------------------------------------------------

		; función en THUMB que recibe como parámetro la matriz mat
		; traspone dicha matriz y actualiza su valor
		; r0 = @mat
matriz3x3_trasponer_THB
        PUSH {lr}
		STMDB sp!,{r4-r7}
		
		movs r1,#3			; r1=N=3

		movs r2,#0			; r2=0=i
bT1THB	cmp r2,r1
		bge	finT1THB		; salta si i>=N
		
		adds r3,r2,#1		; r3=j=i+1
bT2THB	cmp	r3,r1		
		bge	finT2THB			; salta si j>=N
		
		movs r4,r1
		muls r4,r2,r4
		adds r4,r4,r3
		
		lsls r4,r4,#2		; r4=4*(3*i+j)
		adds r4,r0,r4		; r4=@mat+4*(3*i+j)=@mat[i][j]
		
		movs r5,r1
		muls r5,r3,r5
		adds r5,r2,r5
		
		lsls r5,r5,#2
		adds r5,r0,r5		; r5=@mat+4*(3*j+i)=@mat[j][i]

		LDR r6,[r4]			; r6=mat[i][j]
		LDR r7,[r5]			; r7=mat[j][i]
		
		STR r7,[r4]			; guardamos mat[j][i] en @mat[i][j]
		STR	r6,[r5]			; guardamos mat[i][j] en @mat[j][i]
		
		adds r3,r3,#1		; j++
		b	bT2THB
		
finT2THB	adds r2,r2,#1		; i++
		b	bT1THB
		
finT1THB LDMIA sp!,{r4-r7}
		POP {pc}

	
		END