#pragma once
#include "programa.h"

class programa1 : public programa
{
    public:
        programa1()
        {
            instr.push_back(new read());
            instr.push_back(new read());
            instr.push_back(new add());
            instr.push_back(new write());
        };
};