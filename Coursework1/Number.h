#pragma once

#include <random>

class Number {
public:
	Number();
	Number(int a, int b, int c, int d);
	~Number();
	friend Number operator+(const Number& lhs, const Number& rhs);
	friend std::ostream& operator<<(std::ostream& ostr, const Number& num);
	bool IllegalCN();

private:
	int a, b, c, d;

};

Number::Number() :
	a(rand() % 19 - 9), b(rand() % 19 - 9),
	c(rand() % 19 - 9), d(rand() % 19 - 9)

{	
}

Number::Number(int a, int b, int c, int d) :
	a(a), b(b), c(c), d(d)
{}

Number::~Number(){}

Number operator+(const Number& lhs, const Number& rhs)
{
	/*Number num = Number((lhs.a + rhs.a) % 10, (lhs.b + rhs.b) % 10, (lhs.c + rhs.c) % 10, (lhs.d + rhs.d) % 10);

	if (num.a < 0) { num.a += 10; }
	if (num.b < 0) { num.b += 10; }
	if (num.c < 0) { num.c += 10; }
	if (num.d < 0) { num.d += 10; }
	*/
	int a = (lhs.a + rhs.a) % 10; if (a < 0) a += 10;
	int b = (lhs.b + rhs.b) % 10; if (b < 0) b += 10;
	int c = (lhs.c + rhs.c) % 10; if (c < 0) c += 10;
	int d = (lhs.d + rhs.d) % 10; if (d < 0) d += 10;

	Number n = Number(a, b, c, d);

	return n;
}

std::ostream& operator<<(std::ostream& ostr, const Number& num)
{
	ostr << num.a << ", " << num.b << ", " << num.c << ", " << num.d;
	return ostr;
}

bool Number::IllegalCN()
{
	int count = 0;
	if (a == b) count += 1;
	if (a == c) count += 1;
	if (a == d) count += 1;
	if (b == c) count += 1;
	if (b == d) count += 1;
	if (c == d) count += 1;

	if (count > 0) return true;
	else return false;
}