/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
	int contador1 =0;
	int contador2 =0;
	int contador3 =0;
	int contador4 =0;
	int contador5 =0;
	int contador6 =0;
	

%}

%%
^[ ]*\n {contador2++;contador1++;}
^[ ]*[a-zA-Z]{-}[aeiouAEIOU] {contador6++;contador3++;}
\n {contador1++;}
[0-9]+ {contador4++;contador3++;}
[.,:;]\n {contador5++;contador3++;contador1++;}
. {contador3++;}


%%

int main() {
	//Irene Pascual Albericio, 871627, Sergio Ros Alcázar, 874792
	int error = yylex ();
	printf("\nTL: %d\n", contador1);
	printf("\nTB: %d\n", contador2);
	printf("\nTC: %d\n", contador3);
	printf("\nTN: %d\n", contador4);
	printf("\nTS: %d\n", contador5);
	printf("\nTCons: %d\n", contador6);
}


