#pragma once
#include <stack>
#include <string>
#include <iostream>
using namespace std;

class instruccion
{    
    public:
        instruccion()
        {};

        virtual ~instruccion() = default;

        virtual void operacion(stack<int>& _pila, int& _pc) =0;
        virtual string miNombre() const=0;

};

class add : public instruccion
{
    public:
        add()
        {};

        void operacion(stack<int>& _pila, int& _pc) override;
        string miNombre() const override;
};

class read : public instruccion
{
    public:
        read()
        {};

        void operacion(stack<int>& _pila, int& _pc) override;
        string miNombre() const override;
};


class write : public instruccion
{
    public:
        write()
        {};

        void operacion(stack<int>& _pila, int& _pc) override;
        string miNombre() const override;
};

class push : public instruccion
{
    public:
        int c;
        push(int _c=0)
            : c(_c)
        {};
        
        void operacion(stack<int>& _pila, int& _pc) override;
        string miNombre() const override;
};


class dup : public instruccion
{
    public:
        dup()
        {};

        void operacion(stack<int>& _pila, int& _pc) override;
        string miNombre() const override;
};


class jumpif : public instruccion
{
    public:
        int l;
        jumpif(int _l=0)
            : l(_l)
        {};

        void operacion(stack<int>& pila, int& _pc) override;
        string miNombre() const override;

};

class mul : public instruccion
{
    public:
        mul()
        {};

        void operacion(stack<int>& _pila, int& _pc) override;
        string miNombre() const override;
};

class swapp : public instruccion
{
    public:
        swapp()
        {};

        void operacion(stack<int>& pila, int& _pc) override;
        string miNombre() const override;
};

class over : public instruccion
{
    public:
        over()
        {};

        void operacion(stack<int>& pila, int& _pc) override;
        string miNombre() const override;
};

