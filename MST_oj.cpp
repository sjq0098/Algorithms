#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

struct Edge {
	long long from;
	long long to;
	long long weight;
	long long index;
	Edge(long long F, long long T, long long W, long long I = 0) {
		from = F;
		to = T;
		weight = W;
		index = I;
	}
};

vector<Edge> get_graph(long long num_edge, long long num_node, bool &invalidInput) {
	vector<Edge> graph;
	for (long long i = 0; i < num_edge; i++) {
		long long F, T, W;
		cin >> F >> T >> W;
		if (F < 1 || F > num_node || T < 1 || T > num_node) {
			invalidInput = true;
		}
		Edge new_edge(F - 1, T - 1, W, i);
		graph.push_back(new_edge);
	}
	return graph;
}

class UFSet {
public:
	vector<long long> parent;
	UFSet(long long n) {
		parent.resize(n);
		for (long long i = 0; i < n; i++) {
			parent[i] = i;
		}
	}
	long long find(long long x) {
		while (x != parent[x]) {
			parent[x] = parent[parent[x]];
			x = parent[x];
		}
		return x;
	}
	bool union_set(long long x, long long y) {
		long long fx = find(x);
		long long fy = find(y);
		if (fx == fy) {
			return false;
		}
		parent[fx] = fy;
		return true;
	}
};

bool cmp(const Edge &a, const Edge &b) { // 先比权值，再比原输入顺序
	if (a.weight != b.weight) {
		return a.weight < b.weight;
	}
	return a.index < b.index;
}

vector<Edge> kruskal_min(vector<Edge> graph, long long num_node) {
	vector<Edge> min_mst;
	UFSet uf(num_node);
	for (auto &edge : graph) {
		if (edge.from < 0 || edge.from >= num_node || edge.to < 0 || edge.to >= num_node)
			continue;
		if (uf.union_set(edge.from, edge.to)) {
			min_mst.push_back(edge);
			if (min_mst.size() == num_node - 1) {
				break;
			}
		}
	}
	return min_mst;
}

int main() {
	long long n, m;
	cin >> n >> m;
	bool invalidInput = false;
	vector<Edge> graph = get_graph(m, n, invalidInput);
	
	if (invalidInput) {
		cout << -1 << endl;
		return 0;
	}
	
	sort(graph.begin(), graph.end(), cmp);
	vector<Edge> mst = kruskal_min(graph, n);
	if (mst.size() < n - 1) {
		cout << -1 << endl;
		return 0;
	}
	long long sum = 0;
	long long xor_sum = 0;
	for (auto &edge : mst) {
		sum += edge.weight;
		xor_sum = xor_sum ^ (edge.index + 1);
	}
	cout << sum << " " << xor_sum << endl;
	return 0;
}
