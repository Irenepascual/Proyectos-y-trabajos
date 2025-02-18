#pragma once
#include "programa.h"

class programa2 : public programa
{
    public:
        programa2()
        {
            instr.push_back(new read());
            instr.push_back(new dup());
            instr.push_back(new write());
            instr.push_back(new push(-1));
            instr.push_back(new add());
            instr.push_back(new dup());
            instr.push_back(new jumpif(1));
        };
};