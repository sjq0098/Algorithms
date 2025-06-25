#include<iostream>
#include<string>
#include<vector>
typedef long long ll;
ll MOD=1e9+7;
using namespace std;

int decode_single(const char &x){
	int alpha=0;
	if(x=='*'){
		alpha=9;
	}
	else if(x=='0'){
		alpha=0;
	}
	else if(isdigit(x)){
		alpha=1;
	}
	return alpha;
}

int decode_pair(const char&x,const char &y){
	int beta=0;
	if(x=='*'&&y=='*'){
		beta=15;
	}
	else if(x=='*'&&isdigit(y)){
		if (y >= '0' && y <= '6') {
			beta=2;
		}
		else {
			beta=1;
		}
	}
	else if (isdigit(x) && y=='*') {
		if (x=='1'){ 
			beta=9;
		}
		if (x=='2') {
			beta=6;
		}
	}
	else if (isdigit(x) && isdigit(y)) {
		int v = (x - '0') * 10 + (y - '0');
		if(v>=10&&v<=26){
			beta=1;
		}
	}
	return beta;
}

int main(){
	string input;
	cin>>input;
	ll l=input.length();
	vector<ll> dp(l+1,0);
	dp[0]=1;
	dp[1]=decode_single(input[0]);
	for(int i=2;i<=l;i++){
		ll alpha=decode_single(input[i-1])*dp[i-1]%MOD;
		ll betaa=decode_pair(input[i-2],input[i-1])*dp[i-2]%MOD;
		dp[i]=(alpha+betaa)%MOD;
	}
	cout<<dp[l];
	return 0;
}
