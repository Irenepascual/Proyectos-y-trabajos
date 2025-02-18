/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%{
	int SecDec = 0;
	int SecReg = 0;
	int SecCod = 0;
	int numCom = 0;
	int numReglas = 0;
	int numInst = 0;
%}

%x DECLARACION
%x REGLAS
%x CODIGO
%x LINEAREGLA

%%

^[%][{]						            {BEGIN DECLARACION;}  //comienza la seccion declaracion
<DECLARACION>[/][/].*	      	{numCom++;}           //cuenta cada vez que haya un comentario
<DECLARACION>\n		  	      	{SecDec++;}           //cuenta las lineas de la seccion de declaracion

<DECLARACION>[%][%].	      	{BEGIN REGLAS;SecReg--;}  //comienza la seccion de reglas
<REGLAS>\n				          	{SecReg++;}           //cuenta las lineas de la seccion de reglas
<REGLAS>[;]				          	{numInst++;}          //cuenta las instrucciones
<REGLAS>[/][/].*			        {numCom++;}           //cuenta cada vez que haya un comentario
<REGLAS>[{]			    	      	{BEGIN LINEAREGLA;}   //comienza la zona de posible linea de regla
<REGLAS>[}]				          	{numReglas++;printf("aqui");}  //cuenta la linea de regla

<LINEAREGLA>[}]([/][/].*)*\n	{numReglas++;SecReg++;BEGIN REGLAS;}  //cuenta la linea de regla
<LINEAREGLA>[/][/].*		      {numCom++;BEGIN REGLAS;}       //cuenta cada vez que haya un comentario
<LINEAREGLA>\n			      	  {SecReg++;BEGIN REGLAS;}       //cuenta la seccion de reglas
<LINEAREGLA>['].*[;].*[']	    {BEGIN REGLAS;}                //ommite si hay un ; en un printf
<LINEAREGLA>[;]				        {numInst++;BEGIN REGLAS;}      //cuenta las instrucciones
<LINEAREGLA>[{].*[;].*[}]	    {numInst++;}                   //cuenta las instrucciones


<REGLAS>[%][%]				        {BEGIN CODIGO;SecCod--;}       //comienza la seccion de codigo
<CODIGO>\n					          {SecCod++;}            //cuenta las lineas de la seccion de codigo
<CODIGO>^[}]			          	{SecCod++;}            //cuenta la ultima linea de seccion de codigo
<CODIGO>[/][/].*		        	{numCom++;}            //cuenta cada vez que haya un comentario


%%

int main() {
	int error = yylex ();

	printf("\n L1: %d \n", SecDec);
	printf("\n L2: %d \n", SecReg);
	printf("\n L3: %d \n", SecCod);
	printf("\n C: %d \n", numCom);
	printf("\n R: %d \n", numReglas);
	printf("\n I: %d \n", numInst);


}