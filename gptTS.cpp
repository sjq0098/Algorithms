#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

vector<vector<int>> get_graph(int node_num, int edge_num) {
	vector<vector<int>> graph(node_num);
	for (int i = 0; i < edge_num; ++i) {
		int src, tar;
		cin >> src >> tar;
		graph[src - 1].push_back(tar - 1);
	}
	// 去重并保持顺序
	for (auto& adj : graph) {
		sort(adj.begin(), adj.end());
		adj.erase(unique(adj.begin(), adj.end()), adj.end());
	}
	return graph;
}

vector<int> get_in_degree(const vector<vector<int>>& graph) {
	vector<int> in_degree(graph.size(), 0);
	for (const auto& adj : graph) {
		for (int v : adj) {
			in_degree[v]++;
		}
	}
	return in_degree;
}

void topologicalSort(const vector<vector<int>>& graph) {
	vector<int> in_degree = get_in_degree(graph);
	int n = graph.size();
	priority_queue<int, vector<int>, greater<int>> pq; // 最小堆存储0-based索引
	
	// 初始化入度为0的节点
	for (int i = 0; i < n; ++i) {
		if (in_degree[i] == 0) {
			pq.push(i);
		}
	}
	
	vector<int> order;
	while (!pq.empty()) {
		int u = pq.top();
		pq.pop();
		order.push_back(u + 1); // 转换为原编号输出
		
		for (int v : graph[u]) {
			if (--in_degree[v] == 0) {
				pq.push(v);
			}
		}
	}
	
	if (order.size() != n) {
		cout << "Impossible!" << endl;
	} else {
		for (int num : order) {
			cout << num << " ";
		}
		cout << endl;
	}
}

int main() {
	ios::sync_with_stdio(false); // 加速输入输出
	cin.tie(nullptr);
	
	int D;
	cin >> D;
	while (D--) {
		int N, M;
		cin >> N >> M;
		vector<vector<int>> graph = get_graph(N, M);
		topologicalSort(graph);
	}
	return 0;
}
