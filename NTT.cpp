#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MOD = 998244353;      
const int PRIMITIVE_ROOT = 3;

ll modexp(ll a, ll e) {
	ll result = 1;
	while (e > 0) {
		if (e % 2 == 1) result = (result * a) % MOD;
		a = (a * a) % MOD;
		e /= 2;
	}
	return result;
}

void ntt(vector<ll> &a, bool invert) {
	int n = a.size();
	int logn = 0;
	int pow2 = 1;
	while (pow2 < n) {
		pow2 *= 2;
		logn++;
	}
	
	vector<int> rev(n);
	for (int i = 0; i < n; i++) {
		int x = i;
		int r = 0;
		for (int k = 0; k < logn; k++) {
			int bit = x % 2;    
			r = r * 2 + bit;    
			x = x / 2;         
		}
		rev[i] = r;
	}
	for (int i = 0; i < n; i++) {
		if (i < rev[i]) swap(a[i], a[rev[i]]);
	}
	
	int len = 1;
	while (len < n) {
		ll wlen = modexp(PRIMITIVE_ROOT, (MOD - 1) / (2 * len));
		if (invert) {
			wlen = modexp(wlen, MOD - 2);
		}
		for (int i = 0; i < n; i += 2 * len) {
			ll w = 1;
			for (int j = 0; j < len; j++) {
				ll u = a[i + j];
				ll v = (a[i + j + len] * w) % MOD;
				a[i + j] = u + v < MOD ? u + v : u + v - MOD;
				a[i + j + len] = u - v >= 0 ? u - v : u - v + MOD;
				w = (w * wlen) % MOD;
			}
		}
		len = len * 2;
	}
	
	if (invert) {
		ll inv_n = modexp(n, MOD - 2);
		for (int i = 0; i < n; i++) {
			a[i] = (a[i] * inv_n) % MOD;
		}
	}
}


vector<ll> multiply(const vector<int> &A, const vector<int> &B) {
	int szA = A.size();
	int szB = B.size();
	int n = 1;
	while (n < szA + szB - 1) n = n * 2;
	
	vector<ll> fa(n, 0), fb(n, 0);
	for (int i = 0; i < szA; i++) fa[i] = A[i];
	for (int j = 0; j < szB; j++) fb[j] = B[j];
	
	ntt(fa, false);
	ntt(fb, false);
	for (int i = 0; i < n; i++) {
		fa[i] = (fa[i] * fb[i]) % MOD;
	}
	ntt(fa, true);
	
	fa.resize(szA + szB - 1);
	return fa;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int n, m;
	cin >> n >> m;
	vector<int> F(n + 1), G(m + 1);
	for (int i = 0; i <= n; i++) cin >> F[i];
	for (int j = 0; j <= m; j++) cin >> G[j];
	reverse(F.begin(), F.end());
	reverse(G.begin(), G.end());
	
	vector<ll> H = multiply(F, G);
	int sz = H.size();
	for (int idx = sz - 1; idx >= 0; --idx) {
		cout << H[idx];
		if (idx > 0) cout << ' ';
	}
	return 0;
}

