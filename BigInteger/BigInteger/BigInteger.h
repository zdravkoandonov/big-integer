#include <iostream>
#include <vector>

// not using the LONG_MAX because we do not know the settings of the compiler
// 2^32
#define DIGIT_MAX 4294967296

// long int is at least 32 bits therefore we will use a maximum of 32 bits
// long long int is at least 64 bits therefore we will use a maximum of 64 bits
typedef long long int llong;
typedef unsigned long long int ullong;
typedef unsigned long int ulong;

using std::istream;
using std::ostream;
using std::vector;

class BigInteger
{
private:
	bool positive;
	vector<ulong> digits;

	static const ullong *masks;
	static const ullong* generateMasks();

public:
	BigInteger(long int = 0);
	BigInteger(BigInteger&);
	~BigInteger();
	BigInteger& operator=(long int);
	BigInteger& operator=(BigInteger&);

private:
	void plus(ullong a, ullong b);

public:
	bool isPositive() const;
	int getSize() const;
	ulong getDigit(int index) const;

	friend std::wstring ToString(const BigInteger&);

	friend istream& operator>>(istream&, BigInteger&);
	friend ostream& operator<<(ostream&, BigInteger&);

	BigInteger& operator+(BigInteger&);
	BigInteger& operator+=(BigInteger&);
	BigInteger& operator-(BigInteger&);
	BigInteger& operator-=(BigInteger&);
	BigInteger& operator*(BigInteger&);
	BigInteger& operator*=(BigInteger&);
	BigInteger& operator/(BigInteger&);
	BigInteger& operator/=(BigInteger&);
	BigInteger& operator%(BigInteger&);
	BigInteger& operator%=(BigInteger&);
	bool operator==(const BigInteger&) const;
	bool operator!=(const BigInteger&) const;
	BigInteger& operator<(BigInteger&);
	BigInteger& operator>(BigInteger&);
	BigInteger& operator<=(BigInteger&);
	BigInteger& operator>=(BigInteger&);
	BigInteger& operator[](BigInteger&);

	BigInteger& operator~();
	BigInteger& operator&(BigInteger&);
	BigInteger& operator&=(BigInteger&);
	BigInteger& operator|(BigInteger&);
	BigInteger& operator|=(BigInteger&);
	BigInteger& operator^(BigInteger&);
	BigInteger& operator^=(BigInteger&);
	BigInteger& operator<<(int);
	BigInteger& operator<<=(int);
	BigInteger& operator>>(int);
	BigInteger& operator>>=(int);

};