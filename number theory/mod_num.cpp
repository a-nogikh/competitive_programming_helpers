/*
	Class that encapsulates modulo operations
*/

template<int mod>
class ModNum
{
private:
	int num;

	inline int easy_mod(int value) const
	{
		while (value > mod)
			value -= mod;
		return value;
	}

public:
	ModNum(const int value) : num(value) {
		if (num < 0)
			num += mod * (-num / mod + 1);
		else if (num > mod)
			num = num % mod;
	}

	operator int() const
	{
		return num;
	}

	ModNum& operator=(const ModNum<mod>& rhs)
	{
		this->num = rhs.num;
		return *this;
	}

	ModNum operator +(const ModNum& rhs) const { return ModNum(easy_mod(rhs.num + num)); }

	ModNum& operator +=(const ModNum& rhs)
	{
		this->num = easy_mod(num + rhs.num);
		return *this;
	}

	ModNum operator -(const ModNum& rhs) const { return ModNum(easy_mod(num + mod - rhs.num)); }

	ModNum& operator -=(const ModNum& rhs)
	{
		this->num = easy_mod(num + mod - rhs.num);
		return *this;
	}

	ModNum operator *(const int& rhs) const
	{
		long long x = (long long)num * rhs;
		return ModNum(x < mod ? x : (x % mod));
	}

	ModNum& operator *=(const int& rhs)
	{
		long long x = (long long)num * rhs;
		this->num = x < mod ? (int)x : (x % mod);
		return *this;
	}

	ModNum operator /(const ModNum& rhs) const { return div(rhs); }

	ModNum operator /(const int rhs) const { return div(rhs); }

	ModNum div(const ModNum& other) const
	{
		return (*this) * (other.pow(mod - 2));
	}

	ModNum div(const int& other) const
	{
		return div(ModNum(other));
	}

	inline ModNum pow(const unsigned long long power) const
	{
		ModNum resp = 1;
		unsigned long long power2_value = 1;
		ModNum power2_mod = *this;
		while (power2_value <= power)
		{
			if (power & power2_value)
				resp *= power2_mod;
			power2_mod *= power2_mod;
			power2_value *= 2ULL;
		}

		return resp;
	}

	inline ModNum pow(const int power) const
	{
		return this->pow((const unsigned long long)power);
	}
};

template<int mod>
class FactorialList
{
private:
	vector<ModNum<mod>> precalculated;
public:
	FactorialList()
	{
		precalculated.resize(1, 1);
	}

	inline ModNum<mod> get(int n)
	{
		while (precalculated.size() <= n)
			precalculated.push_back(precalculated.back() * (int)precalculated.size());
		return precalculated[n];
	}

	ModNum<mod> operator[] (const int n)
	{
		return get(n);
	}
};

template<int mod>
class BinomialCalculator
{
private:
	FactorialList<mod> fact;

	class MatrixProxy
	{
		int n;
		BinomialCalculator& outer;
	public: 
		MatrixProxy(int n, BinomialCalculator& outer) : n(n), outer(outer) {}
		ModNum<mod> operator[] (const int k)
		{
			return outer.get(n,k);
		}
	};

public:
	ModNum<mod> get(int n, int k)
	{
		return fact[n] / (fact[k] * fact[n - k]);
	}

	MatrixProxy operator[] (const int n)
	{
		return MatrixProxy(n, *this);
	}
};

template<int mod>
ModNum<mod> pow(ModNum<mod> base, int power)
{
	return base.pow(power);
}

template<int mod>
ModNum<mod> bin_coef(int n, int k)
{
	static BinomialCalculator<mod> calculator;
	return calculator.get(n, k);
}
