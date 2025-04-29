#include<iostream>
#include<queue>
#include<vector>
using namespace std;
vector<int> Gale_Shapley(int n,vector<vector<int>>ml,vector<vector<int>>wl){
	vector<int> anwser(n,-1);
	queue<int> free_man;
	vector<int> nextpos(n,0);
	vector<int> current_partner(n,-1);
	
	for(int i=0;i<n;i++){
		free_man.push(i);
	}
	
	vector<vector<int>> wr(n, vector<int>(n, 0));
	for(int i=0;i<n;i++){
		for(int r=0;r<n;r++){
			int man=wl[i][r];
			wr[i][man]=r;
		}
	}
	while(!free_man.empty()){
		int curr_man=free_man.front();
		free_man.pop();
		int w_index=nextpos[curr_man];
		nextpos[curr_man]++;            
		int w=ml[curr_man][w_index];
		if(current_partner[w]==-1){
			current_partner[w]=curr_man;
		}
		else{
			int p_n=current_partner[w];
			if(wr[w][curr_man] < wr[w][p_n]){
				current_partner[w]=curr_man;
				free_man.push(p_n);
			}
			else{
				free_man.push(curr_man);
			}
		}
	}
	for(int i=0;i<n;i++){
		anwser[current_partner[i]]=i;
	}
	return anwser;
	
}
int main(){
	int n;
	cin>>n;
	vector<vector<int>> ml(n, vector<int>(n));
	vector<vector<int>> wl(n, vector<int>(n));
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cin>>ml[i][j];
			ml[i][j]--;
		}
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cin>>wl[i][j];
			wl[i][j]--;
		}
	}
	vector<int>ans=Gale_Shapley(n,ml,wl);
	for(int i=0;i<n;i++){
		cout<<ans[i]+1<<" ";
	}
	return 0;
}
