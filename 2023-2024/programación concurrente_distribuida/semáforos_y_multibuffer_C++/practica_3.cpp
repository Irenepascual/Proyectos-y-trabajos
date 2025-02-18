
#include "librerias/Semaphore_V4/Semaphore_V4.hpp"
#include "librerias/MultiBuffer/MultiBuffer.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <ctime>
#include <unistd.h>
#include <chrono>

using namespace std;
const int N_CONTROLLERS = 10;
//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};

void pasarTestigo(MultiBuffer<tarea, N_CONTROLLERS>& mBT, Semaphore& pasa1, Semaphore& pasa2, Semaphore* pasa3[], Semaphore& testigo, int& d1, int& d2, int d3[]){
    if ((mBT.numberNews() < N_CONTROLLERS) && d1>0){
        d1--;
        pasa1.signal();
    }
    else if(((mBT.numberNews() == 0)) && d2>0){
            d2--;
            pasa2.signal();
    }
    else {
        int aux = 0;
        bool salir = false;
        while(salir==false && aux < N_CONTROLLERS) {
            if(d3[aux] > 0 && mBT.isNew(aux)) {
                d3[aux]--;
                pasa3[aux]->signal();
                salir = true;
            }
            else {
                aux++;
            }
        }
        if(salir==false) {
            testigo.signal();
        }
    }
}

void mostrarmatriz(float matriz[][3]){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << setw(10) << double((matriz[i][j]));
        }
        cout << endl;
    }

}

//----------------------------------------------------
void masterTask(tarea& p, MultiBuffer<tarea, N_CONTROLLERS>& mBT, Semaphore& testigo, Semaphore& pasa1, Semaphore& pasa2, Semaphore* pasa3[], float matriz[][3], int& d1, int& d2, int d3[]) {
    string nombre="tareas.txt";
    ifstream fich (nombre);
    string aux;
    string aux2;
    //abrimos el fichero "datos.txt"

    if (fich.is_open()){ 
        string tf="TF";
        while (!fich.eof()) {
            getline(fich,aux,',');
            p.tipoTarea=aux;
            getline(fich,aux2,'\n');
            p.cargaDeTrabajo=stof(aux2);
            testigo.wait();
            if (mBT.numberNews() >= N_CONTROLLERS){
                d1++;
                testigo.signal();
                pasa1.wait();
            }
            int pos=0;
            while(mBT.isNew(pos)){
                pos++;
                if (pos>=N_CONTROLLERS)
                {
                    pos=0;
                }
            }
            mBT.put(p,pos);
            pasarTestigo(mBT, pasa1, pasa2, pasa3, testigo, d1, d2, d3);   
        }
        
        testigo.wait();
        if ((mBT.numberNews() != 0)){
            d2++;
            testigo.signal();
            pasa2.wait();
        }
        p={tf, 0};
        for (int i = 0; i < N_CONTROLLERS; i++)
        {
            mBT.put(p,i);
        }
        pasarTestigo(mBT, pasa1, pasa2, pasa3, testigo, d1, d2, d3);

        testigo.wait();

        if ((mBT.numberNews() == 0)){
        }   
        else{
            d2++;
            testigo.signal();
            pasa2.wait();
        }    
        pasarTestigo(mBT, pasa1, pasa2, pasa3, testigo, d1, d2, d3);
        mostrarmatriz(matriz);
        fich.close();
    }
    else{
        cerr << "Error al abrir el fichero. \n" ;
    }
}



//----------------------------------------------------
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id,  MultiBuffer<tarea, N_CONTROLLERS>& mBT, Semaphore& pasa1, Semaphore& pasa2, Semaphore* pasa3[], Semaphore& pasa4, Semaphore& testigo, int& d1, int& d2, int d3[], float matriz[][3]) {
    bool seguir = true;
    while (seguir) { 
        testigo.wait();
        if(!mBT.isNew(id)) {
            d3[id]++;
            testigo.signal();
            pasa3[id]->wait();
        }
        tarea p2;
        p2= mBT.get(id);
        pasarTestigo(mBT, pasa1, pasa2, pasa3, testigo, d1, d2, d3);
        if (p2.tipoTarea!="TF")
        {
            srand(id + time(NULL));
            int random = rand() % (100 + 1);
            if (p2.tipoTarea=="t1")
            {
                pasa4.wait();
                matriz[0][0]++;
                if(random<=95){
                    this_thread::sleep_for(chrono::microseconds(int(p2.cargaDeTrabajo*100)));
                    matriz[0][2] = matriz[2][2] + p2.cargaDeTrabajo;
                    matriz[0][1]++;
                }
                pasa4.signal();
            }
            else if (p2.tipoTarea=="t2")
            {
                pasa4.wait();
                matriz[1][0]++;
                if(random<=93){
                    this_thread::sleep_for(chrono::microseconds(int(p2.cargaDeTrabajo*100)));
                    matriz[1][2] = matriz[2][2] + p2.cargaDeTrabajo;
                    matriz[1][1]++;
                }
                pasa4.signal();
            }
            else if (p2.tipoTarea=="t3")
            {
                pasa4.wait();
                matriz[2][0]++;
                if(random<=90){
                    this_thread::sleep_for(chrono::microseconds(int(p2.cargaDeTrabajo*100)));
                    matriz[2][2] = matriz[2][2] + p2.cargaDeTrabajo;
                    matriz[2][1]++;
                }
                pasa4.signal();
            }
        }
        else{
            seguir=false;
        }
    }
}
//----------------------------------------------------
int main(int argc, char *argv[]) {
    MultiBuffer<tarea,N_CONTROLLERS> mBT; //multi-buffer de tareas
    Semaphore pasa1(0);
    Semaphore pasa2(0);
    Semaphore *pasa3[N_CONTROLLERS];
    Semaphore pasa4(1);
    Semaphore testigo(1);
    int d1=0;
    int d2=0;
    int d3[N_CONTROLLERS];
    thread P[11]; 
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        d3[i] = 0;
        pasa3[i] = new Semaphore(0);
    }
    
    tarea p;
    tarea p2;
    float matriz[3][3];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            matriz[i][j]=0;
        }
    }
    
    for(int i = 0; i < N_CONTROLLERS; i++) {
        P[i] = thread(&controllerTask, i, ref(mBT), ref(pasa1), ref(pasa2), pasa3, ref(pasa4), ref(testigo), ref(d1), ref(d2), d3, matriz);
    }
    P[N_CONTROLLERS] = thread(&masterTask, ref(p), ref(mBT), ref(testigo), ref(pasa1), ref(pasa2), pasa3, matriz, ref(d1), ref(d2), d3);
    for(int i = 0; i <= N_CONTROLLERS; i++) {
        P[i].join();
    }
    return 0;
}