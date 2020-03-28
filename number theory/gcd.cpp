/*
	Fast gcd calculation
	Found somewhere in codeforces blogs
*/

template<typename T>
inline T fast_gcd(T a, T b)
{
	T c;
	while (b)
	{
		c = b;
		b = a % b;
		a = c;
	}
	return a;
}
