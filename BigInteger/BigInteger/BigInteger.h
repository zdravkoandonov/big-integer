#include <iostream>
#include <vector>

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
	// not using the LONG_MAX because we do not know the settings of the compiler
	// 2^32
	static const int DIGIT_MAX = 4294967296;
	static const int DIGIT_SIZE = 32;

	bool positive;
	vector<ullong> bigDigits;
	vector<char> digits;
	
	int base;

	static const ullong *masks;
	static const ullong* generateMasks();

public:
	BigInteger(int = 0, int base = 10);
	BigInteger(const BigInteger&);
	~BigInteger();
	BigInteger& operator=(long int);
	BigInteger& operator=(const BigInteger&);

private:
	void plus(ullong a, ullong b);

public:
	BigInteger getAbs() const;
	bool isPositive() const;
	int getSize() const;
	ulong getBigDigit(int index) const;

private:
	void changeBase();
	void toDec();
	void toHex();

	void generateBigDigits();
	void generateNumberFromBigDigits();

public:
	friend istream& operator>>(istream&, BigInteger&);
	friend ostream& operator<<(ostream&, BigInteger&);

	BigInteger operator+(const BigInteger&) const;
	BigInteger& operator+=(const BigInteger&);
	BigInteger operator-() const;
	BigInteger operator-(const BigInteger&) const;
	BigInteger& operator-=(const BigInteger&);

	BigInteger& subtractSmallerPositiveDec(const BigInteger &smaller);

	BigInteger operator*(const BigInteger&) const;
	BigInteger operator*(char) const;
	BigInteger& operator*=(const BigInteger&);
	BigInteger operator/(const BigInteger&) const;
	BigInteger& operator/=(const BigInteger&);
	BigInteger operator%(const BigInteger&) const;
	BigInteger& operator%=(const BigInteger&);
	bool operator==(const BigInteger&) const;
	bool operator!=(const BigInteger&) const;
	bool operator<(const BigInteger&) const;
	bool operator>(const BigInteger&) const;
	bool operator<=(const BigInteger&) const;
	bool operator>=(const BigInteger&) const;
	int operator[](int) const;

	BigInteger operator~();
	BigInteger operator&(BigInteger&) const;
	BigInteger& operator&=(BigInteger&);
	BigInteger operator|(BigInteger&) const;
	BigInteger& operator|=(BigInteger&);
	BigInteger operator^(BigInteger&) const;
	BigInteger& operator^=(BigInteger&);
	BigInteger operator<<(int) const;
	BigInteger& operator<<=(int);
	BigInteger operator>>(int) const;
	BigInteger& operator>>=(int);

};

istream& operator>>(istream&, BigInteger&);
ostream& operator<<(ostream&, BigInteger&);