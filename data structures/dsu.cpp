/*
	Disjoint set union
*/


struct DsuNode
{
	int parent_id;
	int node_cnt = 1;
	int edge_cnt = 0;
};

class Dsu
{
#define USE_RECURSIVE_DSU_IMPLEMENTATION 1
private:
	vector<DsuNode> nodes;
	vector<int> tmp_stack;
public:
	Dsu(const int cnt)
	{
		nodes.resize(cnt);
		for (int i = 0; i < cnt; i++)
			nodes[i].parent_id = i;
	}

	int find_id(int id)
	{
		assert(id < nodes.size());
#if USE_RECURSIVE_DSU_IMPLEMENTATION
		if (nodes[id].parent_id == id)
			return id;
		return (nodes[id].parent_id = find_id(nodes[id].parent_id));
#else
		if (nodes[id].parent_id == id)
			return id;
		do
		{
			tmp_stack.push_back(id);
			id = nodes[id].parent_id;
		} while (nodes[id].parent_id != id);

		for (const int u : tmp_stack)
			nodes[u].parent_id = id;
		tmp_stack.clear();
		return id;
#endif
	}

	DsuNode& find(const int id)
	{
		return nodes[find_id(id)];
	}

	bool are_same(const int a, const int b)
	{
		return find_id(a) == find_id(b);
	}

	void join(int a, int b)
	{
		a = find_id(a);
		b = find_id(b);
		if (a != b)
		{
			if (nodes[a].node_cnt < nodes[b].node_cnt)
				swap(a, b);
			nodes[b].parent_id = a;

			nodes[a].node_cnt += nodes[b].node_cnt;
			nodes[a].edge_cnt += nodes[b].edge_cnt;
		}
	}
};
