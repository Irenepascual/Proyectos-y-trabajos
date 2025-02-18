/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
#include <math.h>
#include <stdio.h>
int b =10;
%}
%token NUMBER EOL OP CP IG BASE ESCR_BASE BASE10
%start calclist
%token ADD SUB
%token MUL DIV
%%

calclist : /* nada */
	| calclist exp BASE10 EOL { printf("=%d\n", $2); }
	| calclist BASE IG NUMBER EOL { b= $4 ;}
	| calclist exp ESCR_BASE EOL {
		if(b<11 && b >1){
			int num = $2;
			int res = 0;
			int i = 1;
			while (num!=0){
				res = res + (num % b) * i; 
				num = num/b;
				i = i*10;
			}
			printf("= %d \n", res);
		}
		else{
			printf("La base intruducida %b no esta entre 2 y 10", b);
			return 0;
		
		}
	}
	;
	
exp : 	factor
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }
	;
factor : 	factor MUL factorsimple { $$ = $1 * $3; }
		| factor DIV factorsimple { $$ = $1 / $3; }
		| factorsimple 
		;
factorsimple : 	OP exp CP { $$ = $2; }
		| NUMBER
		;
%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}

