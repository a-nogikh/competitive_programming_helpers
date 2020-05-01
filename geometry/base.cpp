/*
	Basic geometric entities.
	Requires C++17
*/

template<class T = int, typename = enable_if<is_signed<T>::value>>
struct Vector2D
{
	using float_return_type = typename conditional<is_floating_point<T>::value, T, double>::type;
	using multiplication_type = typename conditional<is_same<T, int>::value, long long, T>::type;

	T x, y;

	Vector2D(T x, T y) : x(x), y(y) {}
	multiplication_type dot(const Vector2D& other) const { return (multiplication_type)x * other.x + y * other.y; }
	multiplication_type sq_len() const { return dot(*this); }
	float_return_type len() const { return sqrt((float_return_type)sq_len()); }
	template<typename=enable_if<negation<is_same<T, int>::value>>> Vector2D normalize() const { float_return_type len_v = len(); return Vector2D(x / len_v, y / len_v); }

	Vector2D operator + (const Vector2D& rhs) const { return Vector2D(x + rhs.x, y + rhs.y); }
	Vector2D operator - (const Vector2D& rhs) const { return Vector2D(x - rhs.x, y + rhs.y); }
	Vector2D operator * (const T& rhs) const { return Vector2D(x * rhs, y * rhs); }
	Vector2D& operator += (const Vector2D& rhs) { x += rhs.x;  y += rhs.y; return *this; }
	Vector2D& operator -= (const Vector2D& rhs) { x -= rhs.x;  y -= rhs.y; return *this; }

	T projection_on(const Vector2D& other) { return dot(other.normalize()); }
	float_return_type angle_between(const Vector2D& vector_b) const
	{
		/* Angle in radians */
		assert(sq_len() != 0 && vector_b.sq_len() != 0);
		return acos(dot(vector_b) / len() / vector_b.len());
	}

	multiplication_type cross_product(const Vector2D& vector_b) const { return (multiplication_type)x * vector_b.y - (multiplication_type)vector_b.x * y; }

	bool is_collinear(const Vector2D& vector_b) const { return cross_product(vector_b) == 0; }
	bool is_b_clockwise_around_0(const Vector2D& vector_b) const { return cross_product(vector_b) > 0; }
	bool is_b_counter_clockwise_around_0(const Vector2D& vector_b) const { return cross_product(vector_b) < 0; }

	//template<typename=enable_if<is_integral<T>::value>>
	bool operator == (const Vector2D& other) const
	{
		if constexpr (is_integral<T>::value)
		{
			return x == other.x && y == other.y;
		}
		else
		{
			return false;
		}
	}

	bool operator != (const Vector2D& other) const
	{
		return !(*this == other);
	}
};


template<class T = int>
struct Line
{
	using vector_type = typename Vector2D<T>;
	using float_type = typename Vector2D<T>::float_return_type;
	float_type a, b, c;

	Line(const vector_type& v1, const vector_type& v2) {
		assert(v1 != v2);
		a = float_type{ 1 } / (v2.x - v1.x);
		b = -float_type{ 1 } / (v2.y - v1.y);
		c = float_type{ v1.y } / (v2.y - v1.y) - float_type{ v1.x } / (v2.x - v1.x);
		// normalize!
	}
	bool is_intersects(const Line& other) const {}

};
