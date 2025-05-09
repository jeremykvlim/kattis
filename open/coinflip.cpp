#include <bits/stdc++.h>
using namespace std;

void cooley_tukey(int n, vector<complex<double>> &v) {
    static vector<int> rev;
    static vector<complex<double>> twiddles;

    if (rev.size() != n) {
        rev.resize(n);
        for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] | (i & 1) << __lg(n)) >> 1;
    }

    if (twiddles.size() < n) {
        int m = max(2, (int) twiddles.size());
        twiddles.resize(n, 1);

        for (int k = m; k < n; k <<= 1) {
            auto w = polar(1., M_PI / k);
            for (int i = k; i < k << 1; i++) twiddles[i] = i & 1 ? twiddles[i >> 1] * w : twiddles[i >> 1];
        }
    }

    for (int i = 0; i < n; i++)
        if (i < rev[i]) swap(v[i], v[rev[i]]);

    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) {
                auto t = v[i + j + k] * twiddles[j + k];
                v[i + j + k] = v[i + j] - t;
                v[i + j] += t;
            }
}

vector<double> convolve(const vector<double> &a, const vector<double> &b) {
    int n = bit_ceil(a.size() + b.size() - 1);

    vector<complex<double>> dft(n);
    for (int i = 0; i < a.size(); i++) dft[i].real(a[i]);
    for (int i = 0; i < b.size(); i++) dft[i].imag(b[i]);
    cooley_tukey(n, dft);

    for (auto &v : dft) v *= v;
    cooley_tukey(n, dft);
    reverse(dft.begin() + 1, dft.end());

    vector<double> c(a.size() + b.size() - 1, 0.5);
    for (int i = 0; i < c.size(); i++) c[i] *= dft[i].imag() / n;
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<double>> dp(n);
    for (auto &poly : dp) {
        double x;
        cin >> x;
        x /= 100;

        poly = {1 - x, x};
    }

    for (int i = 1; i < n; i <<= 1)
        for (int j = 0; j < n - i; j += i << 1) dp[j] = convolve(dp[j], dp[i + j]);

    for (int k = 0; k <= n; k++) cout << fixed << setprecision(6) << dp[0][k] << " ";
}
