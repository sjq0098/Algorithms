#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct HopcroftKarp {
	int nL, nR;
	vector<vector<int>> adj;
	vector<int> dist, pairU, pairV;
	HopcroftKarp(int _nL, int _nR)
	: nL(_nL), nR(_nR),
	adj(nL+1), dist(nL+1),
	pairU(nL+1, 0), pairV(nR+1, 0) {}
	
	void addEdge(int u, int v) {
		// u in [1..nL], v in [1..nR]
		adj[u].push_back(v);
	}
	
	bool bfs() {
		queue<int> Q;
		for (int u = 1; u <= nL; u++) {
			if (pairU[u] == 0) {
				dist[u] = 0;
				Q.push(u);
			} else {
				dist[u] = INF;
			}
		}
		int found = 0;
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			for (int v : adj[u]) {
				if (pairV[v] == 0) {
					// we can potentially grow an augmenting path
					found = 1;
				} else if (dist[pairV[v]] == INF) {
					dist[pairV[v]] = dist[u] + 1;
					Q.push(pairV[v]);
				}
			}
		}
		return found;
	}
	
	bool dfs(int u) {
		for (int v : adj[u]) {
			if (pairV[v] == 0 || (dist[pairV[v]] == dist[u] + 1 && dfs(pairV[v]))) {
				pairU[u] = v;
				pairV[v] = u;
				return true;
			}
		}
		dist[u] = INF;
		return false;
	}
	
	int maxMatching() {
		int matching = 0;
		while (bfs()) {
			for (int u = 1; u <= nL; u++) {
				if (pairU[u] == 0 && dfs(u))
					matching++;
			}
		}
		return matching;
	}
};

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int n, m;
	cin >> n >> m;
	HopcroftKarp hk(n, n);
	vector<pair<int,int>> edges;
	edges.reserve(m);
	
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		// 原图中 u->v
		hk.addEdge(u, v);     // 二分图左侧 u -> 右侧 v'
		edges.emplace_back(u, v);
	}
	
	// 1) 求最大匹配
	int M = hk.maxMatching();
	
	// 2) 最小路径覆盖数
	int k = n - M;
	cout << k << "\n";
	
	// 3) 用 pairU 构造 nxt 数组和入度 indeg
	vector<int> nxt(n+1, 0), indeg(n+1, 0);
	for (int u = 1; u <= n; u++) {
		int v = hk.pairU[u];
		if (v != 0) {
			nxt[u] = v;
			indeg[v]++;
		}
	}
	
	// 4) 所有 indeg[i]==0 的 i 是路径起点，从它一路走下去
	vector<bool> vis(n+1, false);
	for (int i = 1; i <= n; i++) {
		if (indeg[i] == 0) {
			vector<int> path;
			int x = i;
			while (x != 0) {
				path.push_back(x);
				vis[x] = true;
				x = nxt[x];
			}
			// 输出这一条路径
			cout << path.size();
			for (int v : path)
				cout << " " << v;
			cout << "\n";
		}
	}
	
	// （如果有孤立点，不在任何匹配中，也会被上面当作单点路径输出）
	return 0;
}

