#include "rational.h"

// Auxiliares, privadas (para simplificar la fracción)

int mcd(int a, int b)
{
	return ( b==0 ? a : mcd(b,a%b) );
}

void reduce(Rational& r)
{
	int numero;
	numero = mcd(r._num,r._den);
	r._num = (r._num / numero);
	r._den = (r._den / numero);
}

// Inicializadores

void init(Rational& r)
{
	r._num = 0;
	r._den = 1;
}

void init(Rational& r,int num, int den)
{
	r._num = num;
	r._den = den;
	reduce(r);
}

// Entrada/salida

void write(std::ostream& os, const Rational& r)
{
	os << r._num << "/" << r._den;
}

void read(std::istream& is, Rational& r)
{
	char c;
	is >> r._num >> c >> r._den;
}

// Operaciones aritmeticas

Rational add(const Rational& r1, const Rational& r2)
{
	Rational res;
	res._num = (r1._num*r2._den) + (r2._num*r1._den);
	res._den = (r1._den*r2._den);
	reduce(res);
	return res;
}

Rational sub(const Rational& r1, const Rational& r2)
{
	Rational res;
	res._num = (r1._num*r2._den - r2._num*r1._den);
	res._den = r1._den*r2._den;
	reduce(res);
	return res;
}

Rational mul(const Rational& r1, const Rational& r2)
{
	Rational res;
	res._num = (r1._num*r2._num);
	res._den = (r1._den*r2._num);
	reduce(res);
	return res;
}

Rational div(const Rational& r1, const Rational& r2)
{
	Rational res;
	res._num = r1._num*r2._den;
	res._den = r1._den*r2._num;
	reduce(res);
	return res;
}

// Operaciones logicas

bool equal(const Rational& r1, const Rational& r2)
{
	float n1 = float(r1._num)/float(r1._den);
	float n2 = float(r2._num)/float(r2._den);
	return n1==n2;
}

bool lesser_than(const Rational& r1, const Rational& r2)
{
	float n1 = float(r1._num)/float(r1._den);
	float n2 = float(r2._num)/float(r2._den);
	return n1<n2;
}

bool greater_than(const Rational& r1, const Rational& r2)
{
	float n1 = float(r1._num)/float(r1._den);
	float n2 = float(r2._num)/float(r2._den);
	return n1>n2;
}
