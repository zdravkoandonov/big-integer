#include <iostream>
#include "BigInteger.h"

using namespace std;

int main()
{
	BigInteger t1(234), t2(87);
	t1 = ~t1;
	cout << t1 << endl;
	t1 &= t2;
	cout << t1 << endl;
	t1 |= t2;
	cout << t1 << endl;
	t1 ^= t2;
	cout << t1 << endl;

	BigInteger a(123123123), b, c;
	cout << a[0] << endl;
	cout << a << endl;
	cin >> b;
	cin >> c;
	BigInteger d = b, e = c;
	//cout << std::hex;
	cout << b << endl;
	cout << c << endl;
	b += c;
	cout << b << endl;
	cout << d << endl << e << endl;
	d -= e;
	cout << d << endl;

	BigInteger p, q;
	cin >> p >> q;
	p *= q;
	cout << p << endl;

	BigInteger p1, q1;
	cin >> p1 >> q1;
	p1 /= q1;
	cout << p1 << endl;

	BigInteger p2, q2;
	cin >> p2 >> q2;
	cout << p2 / q2 << endl;
	p2 %= q2;
	cout << p2 << endl;

	return 0;
}