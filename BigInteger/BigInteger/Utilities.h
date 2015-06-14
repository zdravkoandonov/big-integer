#pragma once
class Utilities
{
public:
	static int charToInt(char c)
	{
		if (c >= '0' && c <= '9')
			return c - '0';

		// hex digits
		if (c >= 'a' && c <= 'f')
			return 10 + c - 'a';

		return -1;
	}

	static char intToChar(int n)
	{
		if (n >= 0 && n <= 9)
			return n + '0';

		// hex digits
		if (n >= 10 && n <= 15)
			return 'a' + n - 10;

		return 0;
	}
};

