#include <iostream>
#include <queue>
#include<stack>
#include <vector>
using namespace std;

vector<int> Gale_Shapley(int n, const vector<vector<int>> &ml, const vector<vector<int>> &wl) {
	vector<int> answer(n, -1);           // 最终结果：男性 -> 女性
	stack<int> free_man;                 // 存储空闲的男性
	vector<int> nextpos(n, 0);           // 记录每个男性下一次提议的女性索引
	vector<int> current_partner(n, -1);  // 记录每个女性当前匹配的男性 (-1 表示未匹配)
	
	// 所有男性最初均为空闲
	for (int i = n-1; i < n; i++) {
		free_man.push(i);
	}
	
	// 构造女性的优先级映射 wr[w][m] 表示女性 w 对男性 m 的排名（值越小越优先）
	vector<vector<int>> wr(n, vector<int>(n, 0));
	for (int w = 0; w < n; w++) {
		for (int r = 0; r < n; r++) {
			int man = wl[w][r];
			wr[w][man] = r;
		}
	}
	
	// 主循环：处理所有空闲男性的提议
	while (!free_man.empty()) {
		int curr_man = free_man.top();
		free_man.pop();
		int w_index = nextpos[curr_man];  // 获取当前男性的提议位置
		nextpos[curr_man]++;              // 更新提议索引，确保下一次不会重复提同一位女性
		int w = ml[curr_man][w_index];     // 当前提议的女性
		
		// 如果女性 w 还未匹配，则直接匹配
		if (current_partner[w] == -1) {
			current_partner[w] = curr_man;
		}
		else {
			int p_n = current_partner[w];  // 当前与 w 匹配的男性
			// 判断 w 是否更偏好新来的 curr_man
			if (wr[w][curr_man] < wr[w][p_n]) {
				current_partner[w] = curr_man;  // 更新匹配
				free_man.push(p_n);             // 原匹配的男性重新变为空闲
			}
			else {
				free_man.push(curr_man);        // 保持当前匹配，curr_man 继续等待下一个提议机会
			}
		}
	}
	
	// 构造最终匹配结果（男性 -> 女性）
	for (int w = 0; w < n; w++) {
		answer[current_partner[w]] = w;
	}
	
	return answer;
}

int main() {
	int n;
	cin >> n;
	// 初始化 ml 和 wl 的大小
	vector<vector<int>> ml(n, vector<int>(n));
	vector<vector<int>> wl(n, vector<int>(n));
	
	// 输入男性偏好（注意减 1 以转为 0-indexed）
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> ml[i][j];
			ml[i][j]--;  // 转换为 0 索引
		}
	}
	// 输入女性偏好（注意减 1 以转为 0-indexed）
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> wl[i][j];
			wl[i][j]--;  // 转换为 0 索引
		}
	}
	
	vector<int> ans = Gale_Shapley(n, ml, wl);
	for (int i = 0; i < n; i++) {
		cout << ans[i] + 1 << " ";  // 输出时转换回 1-indexed
	}
	
	return 0;
}

