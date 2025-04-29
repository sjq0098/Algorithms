#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include<algorithm>
using namespace std;

vector<vector<int>> get_graph(int node_num, int edge_num) {
	vector<vector<int>> graph;
	graph.resize(node_num);
	for (int i = 0; i < edge_num; i++) {
		int src, tar;
		cin >> src >> tar;
		// 转换为 0-index
		graph[src - 1].push_back(tar - 1);
	}
	// 对每个邻接列表去重
	for (auto& adj : graph) {
		sort(adj.begin(), adj.end());
		auto last = unique(adj.begin(), adj.end());
		adj.erase(last, adj.end());
	}
	return graph;
}

vector<int> get_in_degree(const vector<vector<int>> &graph) {
	int num = graph.size();
	vector<int> in_degree(num, 0);
	for (int i = 0; i < num; i++) {
		for (int j : graph[i]) {
			in_degree[j]++;
		}
	}
	return in_degree;
}

void topologicalSort(const vector<vector<int>> &graph) {
	vector<int> in_degree = get_in_degree(graph);
	int num = graph.size();
	priority_queue<int, vector<int>, greater<int>> minHeap;
	vector<int> topoorder;
	
	for (int i = 0; i < num; i++) {
		if (in_degree[i] == 0) {
			minHeap.push(i);
		}
	}
	
	while (!minHeap.empty()) {
		int u = minHeap.top();
		minHeap.pop();
		topoorder.push_back(u);
		for (int v : graph[u]) {
			in_degree[v]--;
			if (in_degree[v] == 0) {
				minHeap.push(v);
			}
		}
	}
	
	if (topoorder.size() != num) {
		cout << "Impossible!" << endl;
		return;
	}
	
	// 输出时直接使用循环变量
	for (int vertex : topoorder) {
		cout << vertex + 1 << " ";
	}
	cout << endl;
}

int main(){
	int n;
	cin >> n;
	for (int i = 0; i < n; i++){
		int node_num, edge_num;
		cin >> node_num >> edge_num;
		vector<vector<int>> graph = get_graph(node_num, edge_num);
		topologicalSort(graph);
	}
	return 0;
}

