/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
#include <math.h>
#include <stdio.h>
int ac=0;
%}
%token NUMBER EOL OP CP IG
%start calclist
%token ADD SUB DES
%token MUL DIV AC
%%

calclist : /* nada */
	| calclist exp EOL { printf("=%d\n", $2); }
	| calclist AC DES exp EOL { ac= $4 ;}
	| calclist AC { printf("=%d\n", ac);}
	;
	
exp : 	factor
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }
	;
factor : 	factor MUL factorsimple { $$ = $1 * $3; }
		| factor DIV factorsimple { $$ = $1 / $3; }
		| factorsimple 
		;
factorsimple : 	OP exp CP  { $$ = $2; }
		| NUMBER	
		| AC              { $$ = ac; }
		;
%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}
