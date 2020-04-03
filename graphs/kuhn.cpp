/*
	Kuhn's algorithm
	O(N*E)
*/

class KuhnMaxMatch
{
private:
	vector<vector<int>> lhs_edges;
	int right_cnt;
	int current_run = 0; 
	vector<int> curr_rhs_assignment;
	vector<int> curr_visited;

	bool dfs(const int vtx)
	{
		if (curr_visited[vtx] == current_run)
			return false;

		curr_visited[vtx] = current_run;

		for (const int &right_idx : lhs_edges[vtx])
		{
			if (curr_rhs_assignment[right_idx] == -1)
			{
				curr_rhs_assignment[right_idx] = vtx;
				return true;
			}
		}

		for (const int &right_idx : lhs_edges[vtx])
		{
			if (dfs(curr_rhs_assignment[right_idx]))
			{
				curr_rhs_assignment[right_idx] = vtx;
				return true;
			}
		}

		return false;
	}
public:
	KuhnMaxMatch(const int left, const int right)
	{
		lhs_edges.resize(left);
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
		curr_visited.assign(lhs_edges.size(), 0);
		curr_rhs_assignment.assign(right_cnt, -1);
		vector<bool> preprocessed_left(lhs_edges.size(), false);

		// A heuristic: we first greedily find some matching
		for (int i = 0; i < lhs_edges.size(); i++)
		{
			for (const int &right_id : lhs_edges[i])
			{
				if (curr_rhs_assignment[right_id] < 0)
				{
					curr_rhs_assignment[right_id] = i;
					preprocessed_left[i] = true;
					break;
				}
			}
		}

		for (int i = 0; i < lhs_edges.size(); i++)
		{
			if (preprocessed_left[i])
				continue;

			current_run++;
			dfs(i);
		}

		vector<int> lhs_assignment(lhs_edges.size(), -1);
		for (int i = 0; i < curr_rhs_assignment.size(); i++)
			if (curr_rhs_assignment[i] >= 0)
				lhs_assignment[curr_rhs_assignment[i]] = i;

		return lhs_assignment;
	}
};
