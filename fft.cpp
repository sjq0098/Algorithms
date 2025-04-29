#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
using namespace std;

using cd = complex<double>;
const double PI = acos(-1.0);

// 递归实现 
void fft(vector<cd> & a, bool invert) {
	int n = a.size();
	if (n == 1) return;
	
	vector<cd> a0(n / 2), a1(n / 2);
	for (int i = 0; i < n / 2; ++i) {
		a0[i] = a[2 * i];
		a1[i] = a[2 * i + 1];
	}
	
	fft(a0, invert);
	fft(a1, invert);
	
	double sign;
	if (invert) {
		sign = -1.0;
	} else {
		sign = 1.0;
	}
	double ang = 2 * PI / n * sign;
	cd w(1);
	cd wn(cos(ang), sin(ang));
	
	for (int k = 0; k < n / 2; ++k) {
		cd u = a0[k];
		cd v = w * a1[k];
		a[k] = u + v;
		a[k + n / 2] = u - v;
		if (invert) {
			a[k] /= 2;
			a[k + n / 2] /= 2;
		}
		w = w * wn;
	}
}

vector<long long> multiply(const vector<int> & A, const vector<int> & B) {
	int n = 1;
	
	while (n < (int)A.size() + (int)B.size()) {
		n = n * 2;
	}
	
	vector<cd> fa(n), fb(n);
	for (size_t i = 0; i < A.size(); ++i) {
		fa[i] = cd(A[i], 0);
	}
	for (size_t i = 0; i < B.size(); ++i) {
		fb[i] = cd(B[i], 0);
	}
	
	fft(fa, false);
	fft(fb, false);
	
	for (int i = 0; i < n; ++i) {
		fa[i] = fa[i] * fb[i];
	}
	
	fft(fa, true);
	
	vector<long long> C(A.size() + B.size() - 1);
	for (size_t i = 0; i < C.size(); ++i) {
		long long val = static_cast<long long>(round(fa[i].real()));
		C[i] = val;
	}
	return C;
}

int main() {
	int n, m;
	cin >> n >> m;
	
	vector<int> F(n + 1);
	vector<int> G(m + 1);
	for (int i = 0; i <= n; i++) {
		cin >> F[i];
	}
	for (int j = 0; j <= m; j++) {
		cin >> G[j];
	}
	reverse(F.begin(), F.end());
	reverse(G.begin(), G.end());
	
	vector<long long> H = multiply(F, G);
	
	for (int i = (int)H.size() - 1; i >= 0; --i) {
		cout << H[i];
		if (i > 0) {
			cout << ' ';
		}
	}
	return 0;
}
