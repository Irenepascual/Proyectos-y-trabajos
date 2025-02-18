#include "instruccion.h"

/* ADD */
void add::operacion(stack<int>& _pila, int& _pc)
{
    int n1 = _pila.top();
    _pila.pop();
    int n2 = _pila.top();
    _pila.pop();
    int n3 = n1 + n2;
    _pila.push(n3);
    _pc++;
};

string add::miNombre() const
{
    return "add";
}

/* READ */
void read::operacion(stack<int>& _pila, int& _pc)
{
    cout << "? " << flush;
    int num;
    cin >> num;
    _pila.push(num);
    _pc++;
}

string read::miNombre() const
{
    return "read";
}

/* WRITE */
void write::operacion(stack<int>& _pila, int& _pc)
{
    int num = _pila.top();
    _pila.pop();
    cout << num << endl;
    _pc++;
}

string write::miNombre() const
{
    return "write";
}

/* PUSH */
void push::operacion(stack<int>& _pila, int& _pc)
{
    _pila.push(c);
    _pc++;
}

string push::miNombre() const
{
    return "push " + to_string(c);
}

/* DUP */
void dup::operacion(stack<int>& _pila, int& _pc)
{
    int num = _pila.top();
    _pila.push(num);
    _pc++;
}

string dup::miNombre() const
{
    return "dup";
}

/* JUMPIF */
void jumpif::operacion(stack<int>& _pila, int& _pc)
{
    int num = _pila.top();
    _pila.pop();
    if (num >= 0) {
        _pc = l;
    }
    else{
        _pc++;
    }
}

string jumpif::miNombre() const
{
    return "jumpif " + to_string(l);
}

/* MUL */
void mul::operacion(stack<int>& _pila, int& _pc)
{
    int n1 = _pila.top();
    _pila.pop();
    int n2 = _pila.top();
    _pila.pop();
    _pila.push(n1*n2);
    _pc++;
}

string mul::miNombre() const
{
    return "mul";
}

/* SWAP */
void swapp::operacion(stack<int>& _pila, int& _pc)
{
    int num1 = _pila.top();
    _pila.pop();
    int num2 = _pila.top();
    _pila.pop();
    _pila.push(num1);
    _pila.push(num2);
    _pc++;
}

string swapp::miNombre() const
{
    return "swap";
}


/* OVER */
void over::operacion(stack<int>& _pila, int& _pc)
{
    int num1 = _pila.top();
    _pila.pop();
    int num2 = _pila.top();
    _pila.push(num1);
    _pila.push(num2);
    _pc++;
}

string over::miNombre() const
{
    return "over";
}