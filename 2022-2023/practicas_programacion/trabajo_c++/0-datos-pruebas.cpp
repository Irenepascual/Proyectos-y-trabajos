#include <iostream>
#include <fstream>
#include <string.h>
#include "nombres-ficheros.hpp"


using namespace std;

int main(){
    
    string contenido;
    const string direcion16 = "datos/usos-16.csv",direciont1 = "datos/usos-t1.csv", direcion17 = "datos/usos-17.csv",direciont2 = "datos/usos-t2.csv";
    ifstream original(direcion16);
    if (original.is_open())
    {
        ofstream copia(direciont1);
        if (copia.is_open())
        {
            for(unsigned i=0; i< 10; i++)
            {
                getline(original,contenido);
                copia << contenido;
                if(i != (10 -1))
                {
                    copia << endl;
                }
                      
            }
            copia.close();
        }
        else{
            cerr<< "No se ha creado el fichero: " <<direciont1 << endl;
        }

        original.close();    
    }
    else{
        cerr<< "No se puede abrir el fichero de entrada: "<<direcion16 << endl; 
    }

    ifstream original2 (direcion17);
    if (original2.is_open())
    {
        ofstream copia2(direciont2);
        if (copia2.is_open())
        {
            for(unsigned i=0; i< 2000; i++)
            {
                getline(original2,contenido);
                copia2<< contenido;
                if(i != (2000 -1))
                {
                    copia2 << endl;
                }
                    
            }
            copia2.close();
        }
        else{
            cerr<< "No se ha creado el fichero: " <<direciont2 << endl;
        }

        original2.close();    
    }
    else{
        cerr<< "No se puede abrir el fichero de entrada: "<<direcion17<< endl; 
    }



}