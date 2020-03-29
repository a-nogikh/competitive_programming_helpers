/* 
	A Binary Indexed Tree implementation
*/


template<class T>
class Fwt
{
private:
	vector<T> t;
	int size;
public:
	Fwt(int sz)
	{
		size = sz;
		t.resize(sz);
	}

	T sum(int r) {
		T result = 0;
		for (; r >= 0; r = (r & (r + 1)) - 1)
			result += t[r];
		return result;
	}

	T sum(int l, int r)
	{
		T ret = sum(r);
		if (l > 0)
			ret -= sum(l);
		return ret;
	}

	void inc(int i, T delta) {
		for (; i < size; i = (i | (i + 1)))
			t[i] += delta;
	}

};