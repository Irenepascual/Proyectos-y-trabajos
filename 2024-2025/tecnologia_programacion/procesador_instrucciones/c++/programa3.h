#pragma once
#include "programa.h"

class programa3 : public programa
{
    public:
        programa3()
        {
            instr.push_back(new push(1));
            instr.push_back(new read());
            instr.push_back(new swapp());
            instr.push_back(new over());
            instr.push_back(new mul());
            instr.push_back(new swapp());
            instr.push_back(new push(-1));            
            instr.push_back(new add());
            instr.push_back(new dup());
            instr.push_back(new push(-2));
            instr.push_back(new add());
            instr.push_back(new jumpif(2));
            instr.push_back(new swapp());
            instr.push_back(new write());
        };
};