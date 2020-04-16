/*
	A generic segment tree with pointwise updates
*/

template<class T>
struct SegmentTree
{
private:
	vector<T> arr;
	int size;

	inline void assign(T& left, const T& right) const
	{

	}

	inline T merge(const T& left, int left_sz, const T& right, int right_sz) const
	{
		return T();
	}

	void update(const int i, const T& val, const int ll, const int rr, const int idx)
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

	T get_range_value(const int l, const int r, const int ll, const int rr, const int idx)
	{
		if (l == ll && rr == r)
			return arr[idx];

		int m = (ll + rr) / 2;
		if (max(ll, l) <= min(m, r))
		{
			if ((m + 1) <= r && (m + 1) >= l)
			{
				return merge(get_range_value(l, min(m, r), ll, m, idx * 2 + 1), min(m, r) - l + 1,
					get_range_value(max(m + 1, l), r, m + 1, rr, idx * 2 + 2), r - max(m + 1, l) + 1);
			}
			else
			{
				return get_range_value(l, min(m, r), ll, m, idx * 2 + 1);
			}
		}
		else if (max(m + 1, l) <= min(rr, r))
		{
			return get_range_value(max(m + 1, l), r, m + 1, rr, idx * 2 + 2);
		}
		else
			return T();
	}

	void build(const int ll, const int rr, const int idx, const vector<T>& values)
	{
		if (ll == rr)
		{
			arr[idx] = values[ll];
			return;
		}
		int m = (ll + rr) / 2;
		build(ll, m, idx * 2 + 1, values);
		build(m + 1, rr, idx * 2 + 2, values);
		arr[idx] = merge(arr[idx * 2 + 1], m - ll + 1, arr[idx * 2 + 2], rr - m);
	}
public:
	SegmentTree(const int sz, const T default_value = T()) : size(sz) {
		arr.resize(4 * sz, default_value);
	}

	SegmentTree(const vector<T>& values, const T default_value = T())
	{
		size = values.size();
		arr.resize(4 * size);
		build(0, size - 1, 0, values);
	}

	void update(const int i, const T &val) { update(i, val, 0, size - 1, 0); }

	T get_range_value(const int l, const int r) { return get_range_value(l, r, 0, size - 1, 0); }
};