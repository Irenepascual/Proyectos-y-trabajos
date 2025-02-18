#pragma once
#include "instruccion.h"
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class programa
{
    public:
        //vector de instrucciones
        vector<instruccion*> instr;
        //contador de programa
        int pc;
        //pila
        stack<int> pila;
    public:
        // programa(const vector<instruccion*>& _instr)
        //     : instr(_instr)
        // {
        //     pc = 0;
        // }
        programa()
        {
            pc = 0;
        };

        ~programa()
        {
            for (auto i : instr){
                delete i;
            }
        };
        
        void listar() const;
        void ejecutar();
};