/*
  NIP1: Irene Pascual Albericio
  NIP2: Sergio Ros Alc�zar
*/

%%

^((((((aa*bb|bbb(bbb)*aa*bb)(ba*bb)*a|(baa*b|bbb(bbb)*baa*b)b(ba*bb)*a)a*b|(aa*ba|bbb(bbb)*aa*ba|(aa*bb|bbb(bbb)*aa*bb)(ba*bb)*ba*ba)a*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*aa*bb(ba*bb)*aa*b|(bba|bbb(bbb)*bba)a*bb(ba*bb)*aa*b|((baa*b|bbb(bbb)*baa*b)a|(baa*b|bbb(bbb)*baa*b)b(ba*bb)*ba*ba)a*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*aa*bb(ba*bb)*aa*b)a*b|(aa*ba|bbb(bbb)*aa*ba|(aa*bb|bbb(bbb)*aa*bb)(ba*bb)*ba*ba)a*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*b(ba*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*b)*(aa*b|ba*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*aa*bb(ba*bb)*aa*b)a*b|((bba|bbb(bbb)*bba)a*ba|(bba|bbb(bbb)*bba)a*bb(ba*bb)*ba*ba)a*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*aa*bb(ba*bb)*aa*ba*b|((baa*b|bbb(bbb)*baa*b)a|(baa*b|bbb(bbb)*baa*b)b(ba*bb)*ba*ba)a*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*b(ba*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*b)*(aa*b|ba*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*aa*bb(ba*bb)*aa*b)a*b|((bba|bbb(bbb)*bba)a*ba|(bba|bbb(bbb)*bba)a*bb(ba*bb)*ba*ba)a*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*b(ba*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*b)*(aa*ba*b|ba*b((aa*ba|aa*bb(ba*bb)*ba*ba)a*b)*aa*bb(ba*bb)*aa*ba*b))((b|a)a*ba*b)*)(bbb*a|aa*bbb*a)a*b|baa*bbb*aa*b|baa*ba(a|baa*ba)*(bbb*aa*b|baa*bbb*aa*b)|aa*baa*b(aa*baa*b)*(bb*a|aa*bbb*a)a*b(bb*aa*b|aa*bb*aa*b)*)$  {printf("**--%s", yytext);} //acepta cadena de la interseccion de los dos automatas


^(((bbb*a|aa*bbb*a)a*b|baa*bbb*aa*b|baa*ba(a|baa*ba)*(bbb*aa*b|baa*bbb*aa*b)|aa*baa*b(aa*baa*b)*(bb*a|aa*bbb*a)a*b)(bb*aa*b|aa*bb*aa*b)*)$	{printf("--%s", yytext);} //acepta cadena de M2


^((((aa*bb(ba*bb)*a|bbb(bbb)*aa*bb(ba*bb)*a|(aa*baa*b|aa*bb(ba*bb)*ba*baa*b|bbb(bbb)*(aa*baa*b|aa*bb(ba*bb)*ba*baa*b))(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*aa*bb(ba*bb)*a)a*b|(baa*bb|bbb(bbb)*baa*bb)(ba*bb)*aa*b|(aa*baa*b|aa*bb(ba*bb)*ba*baa*b|bbb(bbb)*(aa*baa*b|aa*bb(ba*bb)*ba*baa*b))(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*b(ba*b(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*b)*(a|ba*b(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*aa*bb(ba*bb)*a)a*b)a*b|((baa*ba|bbb(bbb)*baa*ba)a*b|(baa*bb|bbb(bbb)*baa*bb)(ba*bb)*ba*baa*b)(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*aa*bb(ba*bb)*aa*ba*b|(bbaa*b|bbb(bbb)*bbaa*b)b(ba*bb)*aa*ba*b|((baa*ba|bbb(bbb)*baa*ba)a*b|(baa*bb|bbb(bbb)*baa*bb)(ba*bb)*ba*baa*b)(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*b(ba*b(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*b)*(aa*ba*b|ba*b(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*aa*bb(ba*bb)*aa*ba*b)|((bbaa*b|bbb(bbb)*bbaa*b)aa*b|(bbaa*b|bbb(bbb)*bbaa*b)b(ba*bb)*ba*baa*b)(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*aa*bb(ba*bb)*aa*ba*b|((bbaa*b|bbb(bbb)*bbaa*b)aa*b|(bbaa*b|bbb(bbb)*bbaa*b)b(ba*bb)*ba*baa*b)(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*b(ba*b(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*b)*(aa*ba*b|ba*b(aa*baa*b|aa*bb(ba*bb)*ba*baa*b)*aa*bb(ba*bb)*aa*ba*b))((a|b)a*ba*b)*)$	{printf("**%s", yytext);}  //acepta cadena de M1

%%

int main() {
	int error = yylex ();

}

