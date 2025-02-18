/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
#include <math.h>
#include "y.tab.h"
%}
%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"=" {return(IG);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}
"b"	{return (BASE);}
";b" {return (ESCR_BASE);}
";"	{return (BASE10);}
[0-9]+	{
			yylval = atoi(yytext); 
			return(NUMBER);
		}

\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%
















