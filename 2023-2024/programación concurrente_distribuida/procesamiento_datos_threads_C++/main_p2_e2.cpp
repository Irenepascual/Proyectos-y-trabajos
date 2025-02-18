#include <fstream> 
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;
const int N = 512;
const int N_BUSC = 8;
using VectInt = int[N];
int trozo = N / N_BUSC;


//Abrimos el fichero "datos.txt" y si está abierto procede a leer los datos
//que aparecen en el fichero y meterlos en el vector. Además, una vez haya 
//leido todos los datos, indica mediante "datosCargados" que ya han sido 
//cargados todos los datos para que haya sincronización entre los procesos
//coordinar y search.
//Si el fichero no se abre, devuelve error.
void coord(VectInt v, bool& datosCargados, bool VectFin[], int& maxResult, int& indMax, int& indMin){

    ifstream fichero ("datos.txt"); //abrimos el fichero "datos.txt"

    if (fichero.is_open()){ //comprueba que esté abierto
      
        int numDatos = 0;

        while (fichero >> v[numDatos]){ //introducimos los datos en el vector
            numDatos++;
        }
        datosCargados = true; //indicador del proceso search que ya estan cargados todos los datos

        for (int i = 0; i < N_BUSC; i++){
            while (!VectFin[i]){} //Espera a que todos los hilos se hayan ejecutado
        }

        cout << "Veces que aparece: " + to_string(maxResult) + " entre el trozo " + to_string(indMin) + " y " + to_string(indMax) + "\n"; //Escribe por pantalla el numero total de veces que aparece el valor
    }
    else{
        cerr << "No se pudo abrir el fichero \n"; //Error al no poder abrir el fichero "datos.txt"
        
    }
}

//Se encarga de buscar en el trozo que le corresponde (entre i y d, osea, izquierda y derecha), el número
//de veces que aparece el valor que nos daban como dato. Como este proceso se lleva acabo 8 veces y el 
//programa es concurrente, se irá actualizando como máximo, cada vez que se lleve a cabo, si ese ha sido el trozo con mayor 
//número de valores iguales al dato, guardando de este modo la i y la d (para saber el trozo exacto).
void search (const VectInt v, const int i, const int d, const int value, bool& datosCargados, bool VectFin[], int hilos, int& maxResult, int& indMin, int& indMax, atomic_flag& tas){
    while (!datosCargados)
    {
    }
    
    int num=0; 
    for( int j = i ; j <= d; j++){ //recorremos el trozo correspondiente y chequeamos cuantas veces aparece el dato.
            if(v[j] == value){
                num++;
            }
        }

    tas.test_and_set(); //Esto se produce de forma excluyente, por eso utilizamos esta instrucción.
    if (num > maxResult) //Si el número de veces que aparece el dato en ese trozo, es superior al máximo que 
    {                    //llevamos comprobado hasta ese momento, se actualiza el maximo y sus dos indices. 
        maxResult=num;
        indMin = i;
        indMax= d;
    }
    tas.clear(); //Termina la zona de exclusión mutua

    VectFin[hilos] = true; //Indica que ese proceso ya se ha terminado.
}


int main(){
    bool datosCargados=false;
    bool VectFin[N_BUSC];
    int VectCont[N_BUSC];
    thread P[N_BUSC];
    VectInt v;
    int dato;
    int maxResult = 0;
    int indMin = 0;
    int indMax = 0;
    atomic_flag lock_stream = ATOMIC_FLAG_INIT;


    cout << "Introduce un valor: "; //El usuario introduce el valor que quiere buscar en el fichero.
    cin >> dato;
    cout << endl;

    for (int i = 0; i < N_BUSC; i++) //Inicializamos el vector que nos avisa si los hilos han terminado el proceso de search
    {                               
       VectFin[i] = false;
    }

    thread hilo(&coord, v, ref(datosCargados), VectFin, ref(maxResult), ref(indMax), ref(indMin)); //La función coordinar

    for (int i = 0; i < N_BUSC; i++) //Se llevan a cabo todos los hilos en orden.
    {
        int izquierda = i*(trozo); //Indicamos cuales son los extremos del trozo
        int derecha = (((i+1)*trozo)-1);
        if (i==7)
        {
            derecha++; //Si nos encontramos en el último trozo, así lee el último número.
        }
        
        P[i] = thread(&search, v , izquierda, derecha, dato, ref(datosCargados),VectFin,i, ref(maxResult), ref(indMin), ref(indMax), ref(lock_stream)); //La función search
    }

    hilo.join(); 
    for (int i = 0; i < N_BUSC; i++)
    {
        P[i].join();
    }
} 