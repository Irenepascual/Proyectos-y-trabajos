/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%%
^[ ]*\n {;}
^[ ]+ {printf ("    ");}
[ ]+$ {printf ("");}
[ ][ ]+ {printf (" ");}
%%

int main() {
//Irene Pascual Albericio, 871627, Sergio Ros Alcázar, 874792
int error = yylex ();
}