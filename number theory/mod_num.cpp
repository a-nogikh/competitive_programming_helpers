/*
	Class that encapsulates modulo operations
*/

#define MOD 1000000007  // Must be prime!


class ModNum
{
private:
	int num;
	const int mod = MOD;

	inline int easy_mod(int value) const
	{
		while (value > mod)
			value -= mod;
		return value;
	}

public:
	ModNum(const int value): num(value) {
		if (num < 0)
			num += mod * (-num / mod + 1);
		else if (num > mod)
			num = num % mod;
	}
	
	operator int() const
	{
		return num;
	}

	ModNum& operator=(const ModNum& rhs)
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

	ModNum operator *(const int& rhs) const
	{
		long long x = (long long) num * rhs;
		return ModNum(x < mod ? x : (x % mod));
	}

	ModNum& operator *=(const int& rhs)
	{
		long long x = (long long) num * rhs;
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
