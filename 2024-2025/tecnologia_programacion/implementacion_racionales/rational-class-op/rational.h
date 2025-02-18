#pragma once

#include <iostream>
using namespace std;

// Solucion con atributos privados.

class Rational
{
private:
	// Atributos
	int _num;
	int _den;
	
private:
	// Auxiliares
	static int mcd(int a, int b);
	void reduce();

public:
	// Constructores
	Rational();
	Rational(int num, int den);

	// Operadores aritmeticos
	Rational operator+(const Rational& r2) const;
	Rational operator-(const Rational& r2) const;
	friend Rational operator*(const Rational& r1, const Rational& r2);
	friend Rational operator/(const Rational& r1, const Rational& r2);

	// Operadores logicos
	bool operator==(const Rational& r2) const;
	bool operator<(const Rational& r2) const;
	bool operator>(const Rational& r2) const;

	// Friends, en su caso
	friend ostream& operator<<(ostream& os, const Rational& r);
	friend istream& operator>>(istream& is, Rational& r);
};

// Operadores aritmeticos
Rational operator*(const Rational& r1, const Rational& r2);
Rational operator/(const Rational& r1, const Rational& r2);

// Entrada/salida
ostream& operator<<(ostream& os, const Rational& r);
istream& operator>>(istream& is, Rational& r);
