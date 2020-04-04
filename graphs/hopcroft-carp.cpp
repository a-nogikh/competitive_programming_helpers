/*
	Hopcroft carpt algorithm implementation
	Time complexity: O(E*sqrt(V))
*/


class HopcroftCarpMatch
{
private:
	vector<vector<int>> lhs_edges;
	int left_cnt, right_cnt;
	vector<int> bfs_dists;
	vector<int> left_assignment, right_assignment;

	bool bfs()
	{
		queue<int> bfs_q;
		bfs_dists.assign(left_cnt, -1);
		for (int i = 0; i < left_cnt; i++)
		{
			if (left_assignment[i] < 0)
			{
				bfs_dists[i] = 0;
				bfs_q.push(i);
			}
		}

		bool found = false;
		while (bfs_q.size() > 0)
		{
			int id = bfs_q.front(); bfs_q.pop();
			for (int right_id : lhs_edges[id])
			{
				int corr_left = right_assignment[right_id];
				if (corr_left == -1)
				{
					found = true;
					continue;
				}
				if (bfs_dists[corr_left] < 0)
				{
					bfs_dists[corr_left] = bfs_dists[id] + 1;
					bfs_q.push(corr_left);
				}
			}
		}

		return found;
	}

	bool dfs(int v)
	{
		if (bfs_dists[v] < 0)
			return false;
		int my_dist = bfs_dists[v];
		bfs_dists[v] = -1;

		for (int right_i : lhs_edges[v])
		{
			if (right_assignment[right_i] == -1
				|| (bfs_dists[right_assignment[right_i]] == (my_dist + 1)
					&& dfs(right_assignment[right_i])))
			{
				left_assignment[v] = right_i;
				right_assignment[right_i] = v;
				return true;
			}
		}

		return false;
	}
public:
	HopcroftCarpMatch(const int left, const int right)
	{
		lhs_edges.resize(left);
		left_cnt = left;
		right_cnt = right;
	}

	void add_edge(const int left, const int right)
	{
		assert(left >= 0 && left < lhs_edges.size());
		assert(right >= 0 && right < right_cnt);
		lhs_edges[left].push_back(right);
	}

	vector<int> max_matching()
	{
		left_assignment.assign(left_cnt, -1);
		right_assignment.assign(right_cnt, -1);

		while (bfs())
		{
			for (int i = 0; i < left_cnt; i++)
			{
				if (left_assignment[i] == -1)
					dfs(i);
			}
		}

		return left_assignment;
	}
};