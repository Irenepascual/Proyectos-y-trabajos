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

// Entrada/salida

void Rational::write(std::ostream& os) const
{
	os << _num << "/" << _den;
}

void Rational::read(std::istream& is)
{
	char c;
	is >> _num >> c >> _den;
}

// Operaciones aritmeticas

Rational Rational::add(const Rational& r2) const
{
	Rational res;
	res._num = _num*r2._den + r2._num*_den;
	res._den = _den*r2._den;
	res.reduce();
	return res;
}

Rational Rational::sub(const Rational& r2) const
{
	Rational res;
	res._num = _num*r2._den - r2._num*_den;
	res._den = _den*r2._den;
	res.reduce();
	return res;
}

Rational Rational::mul(const Rational& r2) const
{
	Rational res;
	res._num = _num*r2._num;
	res._den = _den*r2._den;
	res.reduce();
	return res;
}

Rational Rational::div(const Rational& r2) const
{
	Rational res;
	res._num = _num*r2._den;
	res._den = _den*r2._num;
	res.reduce();
	return res;
}

// Operaciones logicas

bool Rational::equal(const Rational& r2) const
{
	float n1 = float(_num)/float(_den);
	float n2 = float(r2._num)/float(r2._den);
	return n1==n2;
}

bool Rational::lesser_than(const Rational& r2) const
{
	float n1 = float(_num)/float(_den);
	float n2 = float(r2._num)/float(r2._den);
	return n1<n2;
}

bool Rational::greater_than(const Rational& r2) const
{
	float n1 = float(_num)/float(_den);
	float n2 = float(r2._num)/float(r2._den);
	return n1>n2;
}
