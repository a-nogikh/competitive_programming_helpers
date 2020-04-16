/*
	A segment tree implementation that supports range update operations
*/

template<class T>
struct SegmentTree
{
private:
	vector<T> arr;
	vector<bool> needs_push;
	vector<T> to_push;
	int size;

	inline void assign(T& left, const T& right, int left_sz) const
	{
		/* `left` spans `left_sz` elements. Assign `right` to it. */
		throw "Not implemented";
	}

	inline T merge(const T& left, int left_sz, const T& right, int right_sz) const
	{
		throw "Not implemented";
	}

	void push(const int i, const int sz)
	{
		if (!needs_push[i])
			return;
		needs_push[i] = false;
		
		if (i * 2 + 2 < to_push.size())
		{
			needs_push[i * 2 + 1] = needs_push[i * 2 + 2] = true;

			assign(arr[i * 2 + 1], to_push[i], (sz + 1) / 2);
			assign(arr[i * 2 + 2], to_push[i], sz / 2);
			assign(to_push[i * 2 + 1], to_push[i], 1);
			assign(to_push[i * 2 + 2], to_push[i], 1);
		}
		to_push[i] = T();
	}

	void build(int ll, int rr, int idx, const vector<T>& values)
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

	void update(int i, const T& val, int ll, int rr, int idx)
	{
		if (ll == rr)
		{
			assign(arr[idx], val, 1);
			return;
		}

		push(i, rr - ll + 1);
		int m = (ll + rr) / 2;
		if (i <= m)
			update(i, val, ll, m, idx * 2 + 1);
		else
			update(i, val, m + 1, rr, idx * 2 + 2);

		arr[idx] = merge(arr[idx * 2 + 1], m - ll + 1, arr[idx * 2 + 2], rr - m);
	}

	void range_update(int l, int r, int ll, int rr, int idx, const T& val)
	{
		if (l > r)
			return;

		if (l == ll && rr == r)
		{
			assign(arr[idx], val, rr - ll + 1);
			assign(to_push[idx], val, 1);
			needs_push[idx] = true;
			return;
		}
		int m = (ll + rr) / 2;
		push(idx, rr - ll + 1);

		range_update(l, min(m, r), ll, m, idx * 2 + 1, val);
		range_update(max(m + 1, l), r, m + 1, rr, idx * 2 + 2, val);
		arr[idx] = merge(arr[idx * 2 + 1], m - ll + 1, arr[idx * 2 + 2], rr - m);
	}

	T get_range_value(int l, int r, int ll, int rr, int idx)
	{
		if (l == ll && rr == r)
			return arr[idx];

		push(idx, rr - ll + 1);
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
public:
	SegmentTree(const int sz, const T default_value=T()) : size(sz) {
		arr.resize(4 * sz, default_value);
		needs_push.resize(4 * sz, false);
		to_push.resize(4 * sz, default_value);
	}

	SegmentTree(const vector<T> &values, const T default_value=T())
	{
		size = values.size();
		arr.resize(4 * size);
		needs_push.resize(4 * size, false);
		to_push.resize(4 * size, default_value);
		build(0, size - 1, 0, values);
	}

	void update(const int i, const T &val) { update(i, val, 0, size - 1, 0); }

	void range_update(const int l, const int r, const T &val) {  range_update(l, r, 0, size - 1, 0, val); }

	T get_range_value(const int l, const int r) { return get_range_value(l, r, 0, size - 1, 0); }
};
