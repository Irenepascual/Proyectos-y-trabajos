/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

multiplode3		    	I((H*VH*H*VH*H*VH*)+|H*)F
parnomultiplode3		I((H*VH*H*VH*)+|H*)F
imparnomultiplode3	I(H*V*H*+|H*)F

%%
{multiplode3}		      printf("++%s++",yytext);
{parnomultiplode3}  	printf("+%s+",yytext);
{imparnomultiplode3}	printf("-%s-",yytext);
%%

int main(){
	//Irene Pascual Albericio, 871627, Sergio Ros Alcázar, 874792
	printf("\n");
	int error = yylex();
}