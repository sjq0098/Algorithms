#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// 定义任务结构体
struct Task {
	int id;           // 任务编号
	int duration;     // 任务持续时间
};

// 示例中的图使用邻接表表示
// n：任务数量
// tasks：任务列表（索引对应任务编号，从0到n-1）
// adj：邻接表，表示每个任务后继任务的编号
// indegree：每个任务的入度，用于拓扑排序
// 这里假设边的含义为：u -> v表示任务u完成后，任务v才能开始

void criticalPath(int n, const vector<Task>& tasks, const vector<vector<int>>& adj) {
	vector<int> indegree(n, 0);
	// 记录每个任务的最早开始时间（EST）
	vector<int> est(n, 0);
	// 记录每个任务的最早结束时间（EFT）
	vector<int> eft(n, 0);
	
	// 计算各节点的入度
	for (int u = 0; u < n; u++) {
		for (int v : adj[u]) {
			indegree[v]++;
		}
	}
	
	// 拓扑排序计算最早开始/结束时间
	queue<int> q;
	// 入度为0的节点可以直接入队
	for (int i = 0; i < n; i++) {
		if (indegree[i] == 0)
			q.push(i);
	}
	
	// 拓扑排序遍历
	vector<int> topo; // 保存拓扑序列
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		topo.push_back(u);
		eft[u] = est[u] + tasks[u].duration;
		
		// 放宽邻接边的节点
		for (int v : adj[u]) {
			// 更新v的最早开始时间
			est[v] = max(est[v], eft[u]);
			// 减少入度
			indegree[v]--;
			if (indegree[v] == 0)
				q.push(v);
		}
	}
	
	// 项目的最早完成时间为所有任务的最早结束时间中的最大值
	int projectTime = 0;
	for (int i = 0; i < n; i++)
		projectTime = max(projectTime, eft[i]);
	
	cout << "项目最短完工时间: " << projectTime << endl;
	
	// 计算最晚开始和最晚结束时间
	vector<int> lft(n, projectTime);
	vector<int> lst(n, 0);
	// 逆向遍历拓扑序列计算最晚时间
	for (int i = n - 1; i >= 0; i--) {
		int u = topo[i];
		lst[u] = lft[u] - tasks[u].duration;
		for (int v : adj[u]) {
			// u任务必须在v任务最晚开始之前结束
			lft[u] = min(lft[u], lst[v]);
			lst[u] = lft[u] - tasks[u].duration;
		}
	}
	
	// 输出任务的时间参数以及总时差
	cout << "\n任务编号\t持续时间\tEST\tEFT\tLST\tLFT\t总时差\n";
	for (int i = 0; i < n; i++) {
		int totalFloat = lst[i] - est[i];
		cout << i << "\t\t" << tasks[i].duration << "\t\t" 
		<< est[i] << "\t" << eft[i] << "\t" 
		<< lst[i] << "\t" << lft[i] << "\t" 
		<< totalFloat;
		if(totalFloat == 0) cout << "\t(关键任务)";
		cout << endl;
	}
	
	// 找出关键路径
	// 方法之一：从入度为0的任务开始，一直延伸，只选择总时差为0的任务
	cout << "\n关键路径: ";
	for (int i = 0; i < topo.size(); i++) {
		int u = topo[i];
		// 如果当前任务总时差为0，则认为它在关键路径上
		if (lst[u] - est[u] == 0)
			cout << u << " ";
	}
	cout << endl;
}

int main() {
	// 示例：构造一个简单的任务网络
	// 任务数目
	int n = 6;
	// 定义任务及持续时间（任务编号从0到5）
	vector<Task> tasks = {
		{0, 3}, // 任务0持续时间3
		{1, 2}, // 任务1持续时间2
		{2, 4}, // 任务2持续时间4
		{3, 2}, // 任务3持续时间2
		{4, 3}, // 任务4持续时间3
		{5, 1}  // 任务5持续时间1
	};
	
	// 定义邻接表表示任务依赖关系
	// 例如：0->1 表示任务0必须在任务1之前完成
	vector<vector<int>> adj(n);
	// 构造依赖关系：
	// 0 -> 1, 0 -> 2
	// 1 -> 3
	// 2 -> 3, 2 -> 4
	// 3 -> 5
	// 4 -> 5
	adj[0] = {1, 2};
	adj[1] = {3};
	adj[2] = {3, 4};
	adj[3] = {5};
	adj[4] = {5};
	// 节点5没有后续任务
	
	// 求解关键路径
	criticalPath(n, tasks, adj);
	
	return 0;
}


