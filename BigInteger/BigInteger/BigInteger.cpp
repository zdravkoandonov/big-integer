#include "BigInteger.h"
#include "Utilities.h"
#include <string>
#include <sstream>
#include <climits>

using std::string;

const ullong *BigInteger::masks = generateMasks();

const ullong* BigInteger::generateMasks()
{
	ullong *masks = new ullong[65];
	masks[0] = 0;
	for (int i = 1; i <= 64; i++)
		masks[i] = masks[i - 1] * 2 + 1;
	return masks;
}

BigInteger::BigInteger(int number, int base)
	: base(base)
{
	positive = (number >= 0);
	if (number == 0)
	{
		digits.push_back('0');
	}
	else
	{
		while (number != 0)
		{
			digits.push_back(Utilities::intToChar(number % base));
			number /= base;
		}
	}

	toDec();
}

BigInteger::BigInteger(const BigInteger& other)
{
	base = other.base;
	positive = other.isPositive();
	int size = other.digits.size();
	for (int i = 0; i < size; i++)
		digits.push_back(other.digits[i]);
}

BigInteger::~BigInteger()
{

}

BigInteger& BigInteger::operator=(long int number)
{
	digits.clear();
	base = 10;
	// should you lower the capacity?
	if (number == 0)
	{
		digits.push_back('0');
	}
	else
	{
		while (number != 0)
		{
			digits.push_back(Utilities::intToChar(number % 10));
			number /= 10;
		}
	}
	positive = (number >= 0);
	return *this;
}

BigInteger& BigInteger::operator=(const BigInteger& other)
{
	digits.clear();
	this->base = other.base;
	// should you lower the capacity?
	positive = other.isPositive();
	int size = other.digits.size();
	for (int i = 0; i < size; i++)
		digits.push_back(other.digits[i]);
	toDec();
	return *this;
}

istream& operator>>(istream& in, BigInteger &number)
{
	number.digits.clear();

	string s;
	in >> s;

	int firstDigitAt = 0;
	number.base = 10;

	if (s[0] == '-')
	{
		number.positive = false;
		firstDigitAt++;
	}

	if (s[firstDigitAt] == '0' && s[firstDigitAt + 1] == 'x')
	{
		number.base = 16;
		firstDigitAt += 2;
	}

	for (int i = s.size() - 1; i >= firstDigitAt; i--)
	{
		number.digits.push_back(s[i]);
	}

	number.toDec();

	return in;
}

ostream& operator<<(ostream& out, BigInteger &number)
{
	std::ios_base::fmtflags currentFlags = out.flags();

	if ((currentFlags & out.hex) == out.hex)
	{
		// print in hex
		number.toHex();
		if (!number.isPositive())
			out << "-";
		out << "0x";
		int size = number.digits.size();
		for (int i = size - 1; i >= 0; i--)
			out << number.digits[i];
	}
	else
	{ // default
		// print in dec
		number.toDec();
		if (!number.isPositive())
			out << "-";
		int size = number.digits.size();
		for (int i = size - 1; i >= 0; i--)
			out << number.digits[i];
	}

	// reset to dec
	number.toDec();

	return out;
}

bool BigInteger::isPositive() const
{
	return positive;
}

BigInteger BigInteger::getAbs() const
{
	BigInteger n = (*this);
	n.positive = true;
	return n;
}

int BigInteger::getSize() const
{
	return digits.size();
}

ulong BigInteger::getBigDigit(int index) const
{
	return bigDigits[index];
}

void BigInteger::changeBase()
{
	if (base == 10)
	{
		// change to hex
		BigInteger hexNumber(0), decNumber = *this;
		hexNumber.digits.clear();
		if (decNumber != BigInteger(0))
		{
			while (decNumber.getAbs() > 0)
			{
				BigInteger bigIntRemainder = decNumber % 16;
				decNumber /= 16;
				int numberToPush = Utilities::charToInt(bigIntRemainder.digits[0]);
				if (bigIntRemainder.digits.size() > 1)
					numberToPush += (Utilities::charToInt(bigIntRemainder.digits[1])*10);
				hexNumber.digits.push_back(Utilities::intToChar(numberToPush));
			}
		}
		else
		{
			hexNumber.digits.push_back('0');
		}
		this->base = 16;
		this->digits = hexNumber.digits;
	}
	else if (base == 16)
	{
		// change to dec
		BigInteger t(0, 10);
		int size = digits.size();
		BigInteger powOf16(1);
		for (int i = 0; i < size; i++)
		{
			t += (powOf16 * digits[i]);
			powOf16 *= 16;
		}
		t.positive = this->positive;
		*this = t;
	}
}

void BigInteger::toDec()
{
	if (base == 16)
	{
		changeBase();
	}
}

void BigInteger::toHex()
{
	if (base == 10)
	{
		changeBase();
	}
}

void BigInteger::generateBigDigits()
{
	toHex();

	int digitsPerBigDigit = DIGIT_SIZE / 4; // 16 = 2 ^ 4
	int size = digits.size();
	bigDigits.clear();
	for (int i = 0; i < size; i++)
	{
		ullong powOf16 = 1;
		ullong bigDigitToPush = 0;
		for (int j = 0; j < digitsPerBigDigit && i < size; i++, j++)
		{
			bigDigitToPush += powOf16 * Utilities::charToInt(digits[i]);
			powOf16 *= 16;
		}
		bigDigits.push_back(bigDigitToPush);
	}

	toDec();
}

void BigInteger::generateNumberFromBigDigits()
{
	BigInteger number(0);
	number.base = 16;
	int bigDigitsSize = bigDigits.size();
	for (int i = 0; i < bigDigitsSize; i++)
	{
		ullong d = bigDigits[i];
		for (int j = 0; j < DIGIT_SIZE / 4; j++)
		{
			number.digits.push_back(Utilities::intToChar(d % 16));
			d /= 16;
		}
	}

	// remove unnecessary zeroes
	while (number.digits.back() == '0' && number.digits.size() > 1)
		number.digits.pop_back();

	*this = number;
}

bool BigInteger::operator==(const BigInteger& other) const
{
	if (isPositive() != other.isPositive() || digits.size() != other.digits.size())
		return false;

	int size = digits.size();
	for (int i = 0; i < size; i++)
		if (digits[i] != other.digits[i])
			return false;
	return true;
}

bool BigInteger::operator!=(const BigInteger& other) const
{
	return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const
{
	if (isPositive() && !other.isPositive())
		return false;
	if (!isPositive() && other.isPositive())
		return true;

	bool bothPositive = isPositive(); // both are the same

	int size1 = digits.size(), size2 = other.digits.size();
	if (size1 < size2)
		return true;
	if (size1 > size2)
		return false;

	// size1 == size2
	for (int i = size1 - 1; i >= 0; i--)
	{
		if (bothPositive && digits[i] > other.digits[i])
			return false;
		if (bothPositive && digits[i] < other.digits[i])
			return true;
		if (!bothPositive && digits[i] < other.digits[i])
			return false;
		if (!bothPositive && digits[i] > other.digits[i])
			return true;
	}

	return false; // they are equal
}

bool BigInteger::operator>(const BigInteger& other) const
{

	if (isPositive() && !other.isPositive())
		return true;
	if (!isPositive() && other.isPositive())
		return false;

	bool bothPositive = isPositive(); // both are the same

	int size1 = digits.size(), size2 = other.digits.size();
	if (size1 < size2)
		return false;
	if (size1 > size2)
		return true;

	// size1 == size2
	for (int i = size1 - 1; i >= 0; i--)
	{
		if (bothPositive && digits[i] > other.digits[i])
			return true;
		if (bothPositive && digits[i] < other.digits[i])
			return false;
		if (!bothPositive && digits[i] < other.digits[i])
			return true;
		if (!bothPositive && digits[i] > other.digits[i])
			return false;
	}

	return false; // they are equal
}

bool BigInteger::operator<=(const BigInteger& other) const
{
	return !(*this > other);
}

bool BigInteger::operator>=(const BigInteger& other) const
{
	return !(*this < other);
}

int BigInteger::operator[](int index) const
{
	return Utilities::charToInt(digits[index]);
}

BigInteger BigInteger::operator+(const BigInteger &other) const
{
	BigInteger t = *this;
	return t += other;
}

BigInteger& BigInteger::operator+=(const BigInteger &other)
{
	if (this->positive && !other.positive)
		return ((*this) -= other.getAbs());
	else if (!this->positive && other.positive)
		return ((*this) = (other - (-(*this))));

	int size1 = digits.size(), size2 = other.digits.size();
	int minSize = (size1 < size2) ? size1 : size2;
	int maxSize = (size1 < size2) ? size2 : size1;
	int res = 0;
	for (int i = 0; i < minSize; i++)
	{
		res += Utilities::charToInt(this->digits[i]) + Utilities::charToInt(other.digits[i]);
		this->digits[i] = Utilities::intToChar(res % 10);
		res /= 10;
	}
	if (size1 < size2)
	{
		for (int i = 0; i < size2 - size1; i++)
			this->digits.push_back(Utilities::intToChar(0));
		for (int i = minSize; i < maxSize; i++)
		{
			res += Utilities::charToInt(this->digits[i]) + Utilities::charToInt(other.digits[i]);
			this->digits[i] = Utilities::intToChar(res % 10);
			res /= 10;
		}
	}
	else
	{
		for (int i = minSize; i < maxSize; i++)
		{
			res += Utilities::charToInt(this->digits[i]);
			this->digits[i] = Utilities::intToChar(res % 10);
			res /= 10;
		}
	}
	if (res > 0)
		digits.push_back(Utilities::intToChar(res));

	return *this;
}

BigInteger BigInteger::operator-() const
{
	BigInteger t = *this;
	t.positive = !t.positive;
	return t;
}

BigInteger BigInteger::operator-(const BigInteger &other) const
{
	BigInteger t = *this;
	return t -= other;
}

BigInteger& BigInteger::operator-=(const BigInteger &other)
{
	if (this->isPositive() != other.isPositive())
	{
		return (*this) += (-other);
	}
	else if (!this->isPositive() && !other.isPositive())
	{
		return (*this) = (-other) - (-(*this));
	}
	else // both are positive
	{
		if ((*this) < other)
		{
			BigInteger t = other;
			(*this) = -(t.subtractSmallerPositiveDec(*this));
		}
		else
		{
			this->subtractSmallerPositiveDec(other);
		}

		return *this;
	}
}

BigInteger& BigInteger::subtractSmallerPositiveDec(const BigInteger &smaller)
{
	int size1 = digits.size(), size2 = smaller.digits.size();
	int minus1 = 0;
	for (int i = 0; i < size2; i++)
	{
		int p = Utilities::charToInt(digits[i]) - Utilities::charToInt(smaller.digits[i]) - minus1;
		minus1 = 0;
		if (p < 0)
		{
			p += 10;
			minus1 = 1;
		}
		digits[i] = Utilities::intToChar(p);
	}
	for (int i = size2; i < size1; i++)
	{
		int p = Utilities::charToInt(digits[i]) - minus1;
		minus1 = 0;
		if (p < 0)
		{
			p += 10;
			minus1 = 1;
		}
		digits[i] = Utilities::intToChar(p);
	}

	while (digits.back() == '0' && digits.size() > 1)
	{
		digits.pop_back();
	}

	return *this;
}

BigInteger BigInteger::operator*(const BigInteger &other) const
{
	BigInteger t = *this;
	return t *= other;
}

BigInteger BigInteger::operator*(char c) const
{
	int n = Utilities::charToInt(c);
	BigInteger t = *this;
	int size = t.digits.size();
	int res = 0;
	for (int i = 0; i < size; i++)
	{
		res += Utilities::charToInt(t.digits[i]) * n;
		t.digits[i] = Utilities::intToChar(res % 10);
		res /= 10;
	}
	if (res > 0)
		t.digits.push_back(Utilities::intToChar(res));
	return t;
}

BigInteger& BigInteger::operator*=(const BigInteger &other)
{
	positive = !(this->positive ^ other.positive);

	BigInteger t = (*this * other.digits.back());
	int size = other.digits.size();
	for (int i = size-2; i >= 0; i--)
	{
		// t *= 10;
		t.digits.insert(t.digits.begin(), '0');

		t += ((*this) * other.digits[i]);
	}

	while (t.digits.back() == '0' && t.digits.size() > 1)
		t.digits.pop_back();

	*this = t;

	return *this;
}

BigInteger BigInteger::operator/(const BigInteger &other) const
{
	BigInteger t = *this;
	return t /= other;
}

BigInteger& BigInteger::operator/=(const BigInteger &other)
{
	this->positive = !(this->positive ^ other.positive);

	if (other.getAbs() > (*this).getAbs())
	{
		BigInteger t = 0;
		*this = t;
		return *this;
	}

	int size1 = digits.size(), size2 = other.digits.size();
	BigInteger dividend(0);
	BigInteger result(0);
	for (int i = size1-1; i >= 0; i--)
	{
		dividend *= 10;
		dividend += BigInteger(Utilities::charToInt(digits[i]), 10);
		if (dividend >= other.getAbs())
		{
			int j = 1;
			while (other.getAbs() * BigInteger(j, 10) <= dividend)
				j++;
			result *= 10;
			result += (j-1);
			dividend -= (other.getAbs() * BigInteger(j - 1, 10));
		}
		else
		{
			if (result.digits.back() != '0' || result.digits.size() > 1)
				result *= 10;
		}
	}

	result.positive = this->positive;
	*this = result;

	return *this;
}

BigInteger BigInteger::operator%(const BigInteger &other) const
{
	BigInteger t = *this;
	return t %= other;
}

BigInteger& BigInteger::operator%=(const BigInteger &other)
{
	(*this) -= ((*this) / other) * other;
	return *this;
}

BigInteger BigInteger::operator~()
{
	generateBigDigits();

	int size = bigDigits.size();
	for (int i = 0; i < size; i++)
		bigDigits[i] = ~bigDigits[i];

	generateNumberFromBigDigits();

	return *this;
}

BigInteger BigInteger::operator&(BigInteger &other) const
{
	BigInteger t = *this;
	return t &= other;
}

BigInteger& BigInteger::operator&=(BigInteger &other)
{
	generateBigDigits();
	other.generateBigDigits();

	int size1 = bigDigits.size(), size2 = other.bigDigits.size();
	int minSize = (size1 < size2) ? size1 : size2;
	int maxSize = (size1 < size2) ? size2 : size1;
	for (int i = 0; i < minSize; i++)
	{
		bigDigits[i] &= other.bigDigits[i];
	}
	if (size1 > size2)
	{
		for (int i = minSize; i < maxSize; i++)
		{
			bigDigits.pop_back(); // zeroes
		}
	}

	generateNumberFromBigDigits();

	return *this;
}

BigInteger BigInteger::operator|(BigInteger& other) const
{
	BigInteger t = *this;
	return t |= other;
}

BigInteger& BigInteger::operator|=(BigInteger& other)
{
	generateBigDigits();
	other.generateBigDigits();

	int size1 = bigDigits.size(), size2 = other.bigDigits.size();
	int minSize = (size1 < size2) ? size1 : size2;
	int maxSize = (size1 < size2) ? size2 : size1;
	for (int i = 0; i < minSize; i++)
	{
		bigDigits[i] |= other.bigDigits[i];
	}
	if (size1 < size2)
	{
		for (int i = minSize; i < maxSize; i++)
		{
			bigDigits.push_back(other.bigDigits[i]); // the same as the digits from the other number
		}
	}

	generateNumberFromBigDigits();

	return *this;
}

BigInteger BigInteger::operator^(BigInteger& other) const
{
	BigInteger t = *this;
	return t ^= other;
}

BigInteger& BigInteger::operator^=(BigInteger& other)
{
	generateBigDigits();
	other.generateBigDigits();

	int size1 = bigDigits.size(), size2 = other.bigDigits.size();
	int minSize = (size1 < size2) ? size1 : size2;
	int maxSize = (size1 < size2) ? size2 : size1;
	for (int i = 0; i < minSize; i++)
	{
		bigDigits[i] ^= other.bigDigits[i];
	}
	if (size1 < size2)
	{
		for (int i = minSize; i < maxSize; i++)
		{
			bigDigits.push_back(other.bigDigits[i]); // the same as the digits from the other number
		}
	}

	generateNumberFromBigDigits();

	return *this;
}

BigInteger BigInteger::operator<<(int n) const
{
	BigInteger t = *this;
	return t <<= n;
}

BigInteger& BigInteger::operator<<=(int n)
{
	return *this;
}

BigInteger BigInteger::operator>>(int n) const
{
	BigInteger t = *this;
	return t >>= n;
}

BigInteger& BigInteger::operator>>=(int n)
{
	return *this;
}
