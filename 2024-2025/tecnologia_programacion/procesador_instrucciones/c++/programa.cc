#include "programa.h"

void programa::listar() const
{
    for (int i=0; i<instr.size(); i++) {
        cout << setw(2) << i << "   " << instr[i]->miNombre() << endl;
    }
};

void programa::ejecutar()
{
    while (instr[pc] != nullptr) {
        instr[pc]->operacion(pila,pc);
    }
};