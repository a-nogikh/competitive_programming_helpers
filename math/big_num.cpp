/*
	Arbitrary precision arithmetic
*/

class big_num
{
	static constexpr int BIG_NUMBER_10BASE = 1000000000;
	static constexpr int BIG_NUMBER_DIGITS = 9;

private:
	// [0] - least significant
	vector<int> values;
	bool is_positive = true;

	void add(vector<int>& into, const vector<int>& what) const
	{
		int rem = 0;
		for (int i = 0; i < what.size() || rem > 0; i++)
		{
			if (i < what.size())
				rem += what[i];
			if (i < into.size())
				rem += into[i];
			else
				into.push_back(0);
			into[i] = rem % BIG_NUMBER_10BASE;
			rem /= BIG_NUMBER_10BASE;
		}
	}

	void sub(vector<int>& from, const vector<int>& what) const
	{
		assert(compare(from, what) >= 0);

		int rem = 0;
		for (int i = 0; i < what.size() || rem > 0; i++)
		{
			int sub_val = rem;
			if (i < what.size())
				sub_val += what[i];

			if (from[i] >= sub_val)
			{
				from[i] -= sub_val;
				rem = 0;
			}
			else
			{
				from[i] = BIG_NUMBER_10BASE + from[i] - sub_val;
				rem = 1;
			}
		}

		while (from.size() > 1 && from.back() == 0)
			from.pop_back();
	}

	void simple_mul(vector<int>& values, const int by_what) const
	{
		if (by_what == 0)
		{
			values.resize(1, 0);
			return;
		}
		if (by_what == 1)
			return;

		values.push_back(0);
		for (int i = values.size() - 2; i >= 0; i--)
		{
			long long ret = (long long)values[i] * by_what;
			values[i] = ret % BIG_NUMBER_10BASE;
			values[i + 1] += ret / BIG_NUMBER_10BASE;
		}

		for (int i = 0; i < values.size()-1; i++)
		{
			if (values[i] >= BIG_NUMBER_10BASE)
			{
				values[i + 1] += values[i] / BIG_NUMBER_10BASE;
				values[i] %= BIG_NUMBER_10BASE;
			}
		}
		if (values.back() == 0)
			values.pop_back();
	}

	/* -1 less, 0 equal, 1 greater */
	int compare(const vector<int>& lhs, const vector<int>& rhs) const
	{
		if (lhs.size() < rhs.size())
			return -1;
		if (lhs.size() > rhs.size())
			return 1;

		for (int i = lhs.size() - 1; i >= 0; i--)
		{
			if (lhs[i] > rhs[i])
				return 1;
			if (lhs[i] < rhs[i])
				return -1;
		}

		return 0;
	}

	void increment(const vector<int>& rhs_values, const bool rhs_sign)
	{
		if (is_positive == rhs_sign)
		{
			add(values, rhs_values);
		}
		else
		{
			if (compare(values, rhs_values) >= 0)
			{
				sub(values, rhs_values);
			}
			else
			{
				vector<int> new_values = rhs_values;
				sub(new_values, values);
				values = new_values;
				is_positive = !is_positive;
			}
		}
	}

	big_num(const vector<int> values, bool is_positive) : values(values), is_positive(is_positive) {}

public:
	big_num(big_num&& other) noexcept
	{
		is_positive = other.is_positive;
		other.values.swap(this->values);
	}

	big_num(const big_num& other) = default;

	big_num(long long long_value)
	{
		if (long_value < 0)
		{
			long_value *= -1LL;
			is_positive = false;
		}

		while (long_value > 0)
		{
			values.push_back(long_value % BIG_NUMBER_10BASE);
			long_value /= BIG_NUMBER_10BASE;
		}
	}

	big_num(const string& string_value)
	{
		if (string_value[0] == '-')
		{
			is_positive = false;
		}

		int left_offset = !is_positive;
		int value = 0;
		int power = 1;
		for (int i = string_value.size() - 1; i >= left_offset; i--)
		{
			value += power * (string_value[i] - '0');

			power *= 10;
			if (power >= BIG_NUMBER_10BASE || i == left_offset)
			{
				values.push_back(value);
				value = 0;
				power = 1;
			}
		}
	}

	operator string() const
	{
		string response;
		response.reserve(values.size() * log10(BIG_NUMBER_10BASE));

		if (!is_positive)
			response.push_back('-');

		for (int i = values.size() - 1; i >= 0; i--)
		{
			if (i == (values.size() - 1))
				response.append(to_string(values[i]));
			else
			{
				auto string_val = to_string(values[i]);
				for (int j = 0; j < (BIG_NUMBER_DIGITS - string_val.size()); j++)
				{
					response.push_back('0');
				}
				response.append(string_val);
			}
		}
		return response;
	}

	big_num& operator = (const big_num& other) = default;

	big_num& operator += (const big_num& other)
	{
		increment(other.values, other.is_positive);
		return *this;
	}

	big_num operator + (const big_num& other) const
	{
		big_num ret = *this;
		ret.increment(other.values, other.is_positive);
		return ret;
	}

	big_num& operator -= (const big_num& other)
	{
		increment(other.values, !other.is_positive);
		return *this;
	}

	big_num operator - (const big_num& other) const
	{
		big_num ret = *this;
		ret.increment(other.values, !other.is_positive);
		return ret;
	}

	big_num& operator *= (const int& other)
	{
		simple_mul(values, abs(other));
		is_positive = (is_positive == (other >= 0));
		return *this;
	}

	big_num operator * (const int& other) const
	{
		big_num ret = *this;
		simple_mul(ret.values, abs(other));
		ret.is_positive = is_positive == (other >= 0);
		return ret;
	}

	bool operator > (const big_num& other) const
	{
		if (is_positive != other.is_positive)
			return is_positive;
		return compare(values, other.values) > 0;
	}

	bool operator < (const big_num& other) const
	{
		if (is_positive != other.is_positive)
			return !is_positive;
		return compare(values, other.values) < 0;
	}

	bool operator == (const big_num& other) const
	{
		return is_positive == other.is_positive && compare(values, other.values) == 0;
	}
};
