/*
	Data structure to facilitate convex hull trick implementation.
*/

template<class CoordinateType = long long, class FractionType = long double>
class ConvexHullOptimization
{
	template<class CoordinateType = long long, class FractionType = long double>
	struct Line
	{
		CoordinateType slope, x0;
		FractionType left, right;
		FractionType intersection_point(const Line& other) const
		{
			return static_cast<FractionType>(x0 - other.x0) / static_cast<FractionType>( other.slope - slope );
		}

		CoordinateType evaluate(const CoordinateType x)
		{
			return x * slope + x0;
		}
	};

	deque<Line<CoordinateType, FractionType>> lines;
	FractionType leftmost, rightmost;

public:
	ConvexHullOptimization(FractionType leftmost, FractionType rightmost) : leftmost(leftmost), rightmost(rightmost) {}

	void add_left(CoordinateType slope, CoordinateType x0)
	{
		Line<CoordinateType, FractionType> candidate = { slope, x0, leftmost, rightmost };
		while (lines.size() > 0 && lines.front().intersection_point(candidate) >= lines.front().right)
			lines.pop_front();

		if (lines.size() > 0)
		{
			candidate.right = lines.front().intersection_point(candidate);
			lines.front().left = candidate.right;
		}

		lines.push_front(candidate);
	}

	void add_right(CoordinateType slope, CoordinateType x0)
	{
		// TODO: test!
		Line<CoordinateType, FractionType> candidate = { slope, x0, leftmost, rightmost };
		while (lines.size() > 0 && lines.back().intersection_point(candidate) <= lines.front().left)
			lines.pop_back();

		if (lines.size() > 0)
		{
			candidate.left = lines.front().intersection_point(candidate);
			lines.back().right = candidate.left;
		}

		lines.push_back(candidate);
	}

	CoordinateType evaluate(CoordinateType x)
	{
		assert(lines.size() > 0);
		auto it = partition_point(lines.begin(), lines.end(), [x](auto& line) {
			return line.left <= static_cast<FractionType>(x);
			});
		it--;
		return it->evaluate(x);
	}
};
