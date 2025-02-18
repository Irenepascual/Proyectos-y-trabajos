#include <fstream> 
#include <iostream>
#include <thread>

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
void coord(VectInt vDatos, bool& datosCargados, bool VectFin[], int VectConta[]){

    ifstream fichero ("datos.txt"); //abrimos el fichero "datos.txt"

    if (fichero.is_open()){ //comprueba que esté abierto
      
        int numDatos = 0;
        int veces = 0;

        while (fichero >> vDatos[numDatos]){ //introducimos los datos en el vector
            numDatos++;
        }
        datosCargados = true; //indicador del proceso search que ya estan cargados todos los datos

        for (int i = 0; i < N_BUSC; i++){
            while (!VectFin[i]){} //Espera a que todos los hilos se hayan ejecutado
        }

        for (int i = 0; i < N_BUSC; i++){
            veces = veces + VectConta[i];//Cuenta el numero de veces que ha contado cada hilo el valor dado
        }

        cout << "Veces que aparece: " + to_string(veces) + "\n"; //Escribe por pantalla el numero total de veces que aparece el valor
    }
    else{
        cerr << "No se pudo abrir el fichero \n"; //Error al no poder abrir el fichero "datos.txt"
        
    }
}

//Se encarga de buscar en el trozo que le corresponde (entre i y d, osea, izquierda y derecha), el número
//de veces que aparece el valor que nos daban como dato. 
void search (const VectInt v, const int i, const int d, const int value, int& result, bool& datosCargados, bool VectFin[], int hilos){
    while (!datosCargados)
    {
    }
    //aux(v, i , d, value, result);
    for( int j = i ; j <= d; j++){//recorremos el trozo correspondiente y chequeamos cuantas veces aparece el dato.
        if(v[j] == value){
            result++;
        }
    }
    VectFin[hilos] = true;//Indica que ese proceso ya se ha terminado.
    
}

int main(){
    bool datosCargados=false;
    bool VectFin[N_BUSC];
    int VectCont[N_BUSC];
    thread P[N_BUSC];
    VectInt v;
    int dato;

    cout << "Introduce un valor: ";//El usuario introduce el valor que quiere buscar en el fichero.
    cin >> dato;
    cout << endl;
    for (int i = 0; i < N_BUSC; i++)//Inicializamos el vector que nos avisa si los hilos han terminado el proceso de search
    {
       VectFin[i] = false;
       VectCont[i] = 0;
    }
    thread hilo(&coord, v, ref(datosCargados), VectFin, VectCont);//La función coordinar
    for (int i = 0; i < N_BUSC; i++)//Se llevan a cabo todos los hilos en orden.
    {
        int izquierda = i*(trozo);//Indicamos cuales son los extremos del trozo
        int derecha = (((i+1)*trozo)-1);
        if (i==7)
        {
            derecha++;//Si nos encontramos en el último trozo, así lee el último número.
        }
        
        P[i] = thread(&search, v , izquierda, derecha, dato, ref(VectCont[i]), ref(datosCargados),VectFin,i);//La función search
    }
    hilo.join();
    for (int i = 0; i < N_BUSC; i++)
    {
        P[i].join();
    }
}