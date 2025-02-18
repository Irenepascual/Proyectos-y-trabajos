/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
#include "y.tab.h"
%}
%%
"x"	{return(x);}
"y"	{return(y);}
"z"	{return(z);}
\n	{return(finLinea);}
.	{return(yytext[0]);} 
%%