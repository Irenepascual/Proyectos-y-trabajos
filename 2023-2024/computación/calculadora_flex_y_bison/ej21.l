/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
#include <math.h>
#include "y.tab.h"
extern int b;
%}
%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}
"=" {return(IG);}
"b" {return(B);}


[0-9]+	{yylval = atoi(yytext); return(NUMBER);}
[0-9]+"b"	{
				if(b<11 && b >1){
					yylval = atoi(yytext);
					int res = 0;
					int i = 0;
					while (yylval != 0) {
						if((yylval % 10 ) < b){
							
							res = res + yylval %10 * (pow(b,i));
							yylval = yylval/10;
							i++;
						}
						else{
							i=0;
							printf("El numero en base %d no es correcto", b);
							return(yytext[0]);
						}
					}
					yylval = res;
					return (NUMBER);
				}
				else{
					printf("La base intruducida %b no esta entre 2 y 10", b);
					return(yytext[0]);
				}
			 }
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%
















