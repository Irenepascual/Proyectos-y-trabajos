/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
#include <stdio.h>
%}

%token x y z finLinea
%start iniciar

%%

iniciar: S
		| finLinea

S:	finLinea iniciar
	| C x S
	;

B:	x C y
	|x C y finLinea iniciar
	|x C
	;

C:	x B x
	|x B x finLinea iniciar
	|z finLinea iniciar
	|z
	;
%%

int yyerror(char* s){
	printf("%s\n",s);
	return 0;
}
int main() {
    yyparse();
}
