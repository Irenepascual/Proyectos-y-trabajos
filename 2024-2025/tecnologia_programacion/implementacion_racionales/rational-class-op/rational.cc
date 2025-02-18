#include "rational.h"

// Auxiliares

int Rational::mcd(int a, int b)
{
	return ( b == 0 ? a : mcd(b,a%b));
}

void Rational::reduce()
{
	int numero;
	numero = mcd(this->_num,this->_den);
	this->_num = (this->_num / numero);
	this->_den = (this->_den / numero);
}

// Constructores

Rational::Rational()
{
	_num = 0;
	_den = 1;
}

Rational::Rational(int num, int den)
{
	_num = num;
	_den = den;
	this->reduce();
}

// Operadores aritmeticos

Rational Rational::operator+(const Rational& r2) const
{
	Rational res;
	res._num = _num*r2._den + r2._num*_den;
	res._den = _den*r2._den;
	res.reduce();
	return res;
}

Rational Rational::operator-(const Rational& r2) const
{
	Rational res;
	res._num = _num*r2._den - r2._num*_den;
	res._den = _den*r2._den;
	res.reduce();
	return res;
}

Rational operator*(const Rational& r1, const Rational& r2)
{
	Rational res;
	res._num = r1._num*r2._num;
	res._den = r1._den*r2._den;
	res.reduce();
	return res;
}

Rational operator/(const Rational& r1, const Rational& r2)
{
	Rational res;
	res._num = r1._num*r2._den;
	res._den = r1._den*r2._num;
	res.reduce();
	return res;
}

// Operadores logicos

bool Rational::operator==(const Rational& r2) const
{
	float n1 = float(_num)/float(_den);
	float n2 = float(r2._num)/float(r2._den);
	return n1==n2;
}

bool Rational::operator<(const Rational& r2) const
{
	float n1 = float(_num)/float(_den);
	float n2 = float(r2._num)/float(r2._den);
	return n1<n2;
}

bool Rational::operator>(const Rational& r2) const
{
	float n1 = float(_num)/float(_den);
	float n2 = float(r2._num)/float(r2._den);
	return n1>n2;
}

// Entrada/salida

istream& operator>>(istream& is, Rational& r)
{
	char c;
	is >> r._num >> c >> r._den;
	return is;
}

ostream& operator<<(ostream& os, const Rational& r)
{
	os << r._num << "/" << r._den;
	return os;
}
