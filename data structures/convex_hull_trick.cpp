/*
	Data structures to facilitate convex hull trick implementation.
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
			// TODO: handle parallel lines case
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


template<class CoordinateType = long long, class FractionType = long long>
class ConvexHullDynamic
{
	using LineCoordinateType = typename CoordinateType;
	using LineFractionType = typename FractionType;
	struct Line
	{
		LineCoordinateType slope;
		LineCoordinateType x0;
		mutable LineFractionType left;

		Line(LineCoordinateType slope, LineCoordinateType x0, LineFractionType left) : slope(slope), x0(x0), left(left) { }

		LineFractionType intersect(const Line& other) const
		{
			if (is_integral<LineFractionType>::value)
			{
				auto nom = static_cast<LineCoordinateType>(x0 - other.x0), denom = static_cast<LineCoordinateType>(other.slope - slope);
				return nom / denom - ((nom < 0) == (denom < 0) ? 0 : (nom % denom) != 0);
			}
			else
			{
				return static_cast<LineFractionType>(x0 - other.x0) / static_cast<LineFractionType>(other.slope - slope);
			}
		}

		bool operator< (const Line& rhs) const
		{
			return slope < rhs.slope;
		}

		bool operator< (const LineCoordinateType& rhs) const
		{
			return left < rhs;
		}
	};

	CoordinateType min_value;
	set<Line, std::less<>> lines;
public:
	ConvexHullDynamic(CoordinateType min_coord) : min_value(min_coord) {}
	CoordinateType evaluate(CoordinateType x)
	{
		auto it = lines.lower_bound(x); it--;
		return it->slope * x + it->x0;
	}
	void add_line(CoordinateType slope, CoordinateType x0)
	{
		Line candidate(slope, x0, min_value);
		auto it = lines.lower_bound(candidate);
		if (it != lines.end() && it->slope == slope)
		{
			if (it->x0 >= x0)
				return;
			lines.erase(it++);
		}

		decltype(it) new_it;
		std::tie(new_it, std::ignore) = lines.insert(candidate);

		auto prev_it = new_it;
		while (it != lines.end())
		{
			auto new_left = it->intersect(candidate);
			if (prev_it->left > it->left)
				lines.erase(prev_it++);
			else
				prev_it++;

			if (new_left <= it->left)
				break;
			it->left = new_left;
			it++;
		}

		if (new_it != lines.begin())
		{
			it = std::prev(new_it);
			while (true)
			{
				auto new_left = it->intersect(candidate);
				if (new_left <= it->left)
				{
					if (it != lines.begin())
						lines.erase(it--);
					else
					{
						lines.erase(it);
						break;
					}
				}
				else
				{
					new_it->left = new_left;
					return;
				}
			}
			lines.erase(new_it);
		}

	}
};
