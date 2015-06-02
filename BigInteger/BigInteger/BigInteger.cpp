#include "BigInteger.h"
#include <string>
#include <sstream>
#include <climits>

const ullong *BigInteger::masks = generateMasks();

const ullong* BigInteger::generateMasks()
{
	ullong *masks = new ullong[65];
	masks[0] = 0;
	for (int i = 1; i <= 64; i++)
		masks[i] = masks[i-1] * 2 + 1;
	return masks;
}

BigInteger::BigInteger(long int number)
{
	positive = (number >= 0);
	digits.push_back(abs(number));
}

BigInteger::BigInteger(BigInteger& other)
{
	positive = other.isPositive();
	int size = other.getSize();
	for (int i = 0; i < size; i++)
		digits.push_back(other.getDigit(i));
}

BigInteger::~BigInteger()
{

}

BigInteger& BigInteger::operator=(long int number)
{
	digits.clear();
	// should you lower the capacity?
	digits.push_back(abs(number));
	positive = (number >= 0);
	return *this;
}

BigInteger& BigInteger::operator=(BigInteger& other)
{
	digits.clear();
	// should you lower the capacity?
	positive = other.isPositive();
	int size = other.getSize();
	for (int i = 0; i < size; i++)
		digits.push_back(other.getDigit(i));
	return *this;
}

istream& operator>>(istream& in, BigInteger &number)
{

	return in;
}

ostream& operator<<(ostream& out, BigInteger &number)
{
	//get current basefield
	//int bf = out.basefield;
	// set the output to hexadecimal base
	out << std::hex;
	int size = number.getSize();
	for (int i = size-1; i >= 0; i--)
		out << number.getDigit(i);

	// return the output setting to default
	//out << bf;

	return out;
}

bool BigInteger::isPositive() const
{
	return positive;
}

int BigInteger::getSize() const
{
	return digits.size();
}

ulong BigInteger::getDigit(int index) const
{
	return digits[index];
}

std::wstring ToString(const BigInteger& bigInteger)
{
	std::wstringstream s;	
	int size = bigInteger.digits.size();
	for (int i = 0; i < size; i++)
		s << bigInteger.digits[i]; 
	return s.str();
}

bool BigInteger::operator==(const BigInteger& other) const
{
	int size = digits.size();
	for (int i = 0; i < size; i++)
		if (digits[i] != other.getDigit(i))
			return false;
	return true;
}

bool BigInteger::operator!=(const BigInteger& other) const
{
	return !(*this == other);
}

BigInteger& BigInteger::operator+(BigInteger &other)
{
	return *this += other;
}

BigInteger& BigInteger::operator+=(BigInteger &other)
{
	// ULLONG_MAX
	int size1 = digits.size(), size2 = other.getSize();
	int minSize = (size1 < size2) ? size1 : size2;
	int maxSize = (size1 < size2) ? size2 : size1;
	ullong res = 0;
	for (int i = 0; i < minSize; i++)
	{
		res += this->digits[i] + (ullong)other.getDigit(i);
		this->digits[i] = (ulong)(res % DIGIT_MAX);
		res /= DIGIT_MAX;
	}
	if (size1 < size2)
		for (int i = 0 ; i < size2 - size1; i++)
			this->digits.push_back(0);
	for (int i = minSize; i < maxSize; i++)
	{
		res += this->digits[i] + (ullong)other.getDigit(i);
		this->digits[i] = (ulong)(res % DIGIT_MAX);
		res /= DIGIT_MAX;
	}
	if (res > 0)
		this->digits.push_back(res);

	return *this;
}

BigInteger& BigInteger::operator-(BigInteger &other)
{
	return *this -= other;
}

BigInteger& BigInteger::operator-=(BigInteger &other)
{
	return *this;
}

BigInteger& BigInteger::operator*(BigInteger &other)
{
	return *this *= other;
}

BigInteger& BigInteger::operator*=(BigInteger &other)
{
	return *this;
}

BigInteger& BigInteger::operator/(BigInteger &other)
{
	return *this /= other;
}

BigInteger& BigInteger::operator/=(BigInteger &other)
{
	return *this;
}

BigInteger& BigInteger::operator%(BigInteger &other)
{
	return *this %= other;
}

BigInteger& BigInteger::operator%=(BigInteger &other)
{
	return *this;
}

BigInteger& BigInteger::operator~()
{
	int size = digits.size();
	for (int i = 0; i < size; i++)
		digits[i] = !digits[i];
	return *this;
}

BigInteger& BigInteger::operator&(BigInteger &other)
{
	return *this &= other;
}

BigInteger& BigInteger::operator&=(BigInteger &other)
{
	int thisSize = this->digits.size(), otherSize = other.getSize();
	//if (otherSize > thisSize)
	//{
	//	//not necessary because all next digits will be zeroes!

	//	//for (int i = 0; i < otherSize - this->digits.size(); i++)
	//	//	// the AND will not change these zeroes
	//	//	this->digits.push_back(0); 
	//}
	//else 
	if (otherSize < thisSize)
	{
		for (int i = otherSize+1; i < thisSize; i++)
			// all higher digits are zeroes in other and therefor are zeroes in the result
			this->digits[i] = 0;
	}
	int newSize = (thisSize > otherSize) ? otherSize : thisSize;
	for (int i = 0; i < newSize; i++)
		this->digits[i] &= other.getDigit(i);

	return *this;
}

BigInteger& BigInteger::operator|(BigInteger&)
{
	return *this;
}

BigInteger& BigInteger::operator|=(BigInteger&)
{
	return *this;
}

BigInteger& BigInteger::operator^(BigInteger&)
{
	return *this;
}

BigInteger& BigInteger::operator^=(BigInteger&)
{
	return *this;
}

BigInteger& BigInteger::operator<<(int)
{
	return *this;
}

BigInteger& BigInteger::operator<<=(int)
{
	return *this;
}

BigInteger& BigInteger::operator>>(int)
{
	return *this;
}

BigInteger& BigInteger::operator>>=(int)
{
	return *this;
}
