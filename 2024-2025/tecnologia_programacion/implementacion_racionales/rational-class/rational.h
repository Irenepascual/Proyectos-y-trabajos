#pragma once

#include <iostream>

// Solucion con atributos privados.

class Rational
{
private:
	// Atributos
	int _num, _den;

private:
	// Auxiliares

	static int mcd(int a, int b);
	void reduce();

public:
	// Constructores

	Rational();
	Rational(int num, int den);

	// Entrada/salida

	void write(std::ostream& os) const;
	void read(std::istream& is);

	// Operaciones aritmeticas

	Rational add(const Rational& r2) const;
	Rational sub(const Rational& r2) const;
	Rational mul(const Rational& r2) const;
	Rational div( const Rational& r2) const;

	// Operaciones logicas

	bool equal(const Rational& r2) const;
	bool lesser_than(const Rational& r2) const;
	bool greater_than(const Rational& r2) const;
};
