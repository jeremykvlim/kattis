#include <bits/stdc++.h>
using namespace std;

template <typename T, typename R>
void cooley_tukey(int n, vector<T> &v, R root) {
    static vector<int> rev;
    static vector<T> twiddles;

    if (rev.size() != n) {
        rev.resize(n);
        for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] | (i & 1) << __lg(n)) >> 1;
    }

    if (twiddles.size() < n) {
        int m = max(2, (int) twiddles.size());
        twiddles.resize(n, 1);

        for (int k = m; k < n; k <<= 1) {
            auto w = root(k);
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

template <typename T>
vector<T> convolve(const vector<T> &a, const vector<T> &b) {
    int da = a.size(), db = b.size(), m = da + db - 1, n = bit_ceil((unsigned) m);
    if (n <= 16 || min(da, db) <= __lg(n)) {
        vector<T> p(da), q(db);
        for (int i = 0; i < da; i++) p[i] = a[i];
        for (int i = 0; i < db; i++) q[i] = b[i];
        if (da > db) {
            swap(p, q);
            swap(da, db);
        }

        vector<T> r(m, 0);
        for (int i = 0; i < db; i++) r[i] = q[i];
        for (int i = m - 1; ~i; i--) {
            T v = 0;
            for (int j = 0; j <= min(i, da - 1); j++) v += p[j] * r[i - j];
            r[i] = v;
        }
        return r;
    }

    vector<complex<T>> dft(n);
    for (int i = 0; i < da; i++) dft[i].real(a[i]);
    for (int i = 0; i < db; i++) dft[i].imag(b[i]);

    auto root = [](int k) {
        return polar((T) 1, M_PI / k);
    };

    cooley_tukey(n, dft, root);
    for (auto &v : dft) v *= v;
    cooley_tukey(n, dft, root);
    reverse(dft.begin() + 1, dft.end());

    vector<T> c(m, 0.5);
    for (int i = 0; i < m; i++) c[i] *= dft[i].imag() / n;
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
