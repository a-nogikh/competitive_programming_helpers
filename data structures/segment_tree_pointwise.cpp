/*
	A generic segment tree with pointwise updates
*/

template<class T>
struct SegmentTree
{
private:
	vector<T> arr;
	int size;

	inline void assign(T& left, const T& right)
	{
		
	}

	inline T merge(const T& left, int left_sz, const T& right, int right_sz)
	{
		return T();
	}

public:
	SegmentTree(int sz) : size(sz) {
		arr.resize(4 * sz);
	}

	void update(int i, T val, int ll, int rr, int idx)
	{
		if (ll == rr)
		{
			assign(arr[idx], val);
			return;
		}
		int m = (ll + rr) / 2;
		if (i <= m)
			update(i, val, ll, m, idx * 2 + 1);
		else
			update(i, val, m + 1, rr, idx * 2 + 2);

		arr[idx] = merge(arr[idx * 2 + 1], m - ll + 1, arr[idx * 2 + 2], rr - m);
	}
	void update(int i, T val) { update(i, val, 0, size - 1, 0); }

	T get_range_value(int l, int r, int ll, int rr, int idx)
	{
		if (l > r)
			return T();
		if (l == ll && rr == r)
			return arr[idx];
		int m = (ll + rr) / 2;
		return merge(get_range_value(l, min(m, r), ll, m, idx * 2 + 1), min(m, r) - l + 1,
			get_range_value(max(m + 1, l), r, m + 1, rr, idx * 2 + 2), r - max(m + 1, l) + 1);
	}
	T get_range_value(int l, int r) { return get_range_value(l, r, 0, size - 1, 0); }
};