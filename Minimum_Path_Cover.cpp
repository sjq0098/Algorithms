#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;
const int INF = 1e9+7;
//求最大匹配
struct HopcroftKarp{
	// dist 存放 BFS 中层次距离；
	//pairU[u] 存放左侧 u 当前匹配的右侧顶点编号；
	//pairV[v] 存放右侧 v 当前匹配的左侧顶点编号
	int nL, nR;                                 // 左侧顶点个数与右侧顶点个数
	vector<vector<int>> adj;                    // 邻接表：从左侧 u 到右侧 v 的边
	vector<int> dist, pairU, pairV;    
	HopcroftKarp(int _nL, int _nR)
		: nL(_nL), nR(_nR),
		adj(nL+1), dist(nL+1),
		pairU(nL+1, 0), pairV(nR+1, 0) {}
	
	void addEdge(int u,int v){
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
			int u = Q.front(); 
			Q.pop();
			for (int v : adj[u]) {
				if (pairV[v] == 0) {
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
	int n, m;
	cin >> n >> m;
	HopcroftKarp hk(n, n);
	vector<pair<int,int>> edges;
	edges.reserve(m);
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		hk.addEdge(u, v);     
		edges.emplace_back(u, v);
	}
	
	int M = hk.maxMatching();
	int k = n - M;
	cout << k << endl;
	
	vector<int> nxt(n+1, 0), indeg(n+1, 0);
	for (int u = 1; u <= n; u++) {
		int v = hk.pairU[u];
		if (v != 0) {
			nxt[u] = v;      
			indeg[v]++;     
		}
	}
	
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
			cout << path.size();
			for (int v : path)
				cout << " " << v;
			cout << endl;
		}
	}
	return 0;
}
