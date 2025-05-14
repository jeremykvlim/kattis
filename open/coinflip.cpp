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
vector<complex<T>> fft(int n, const vector<complex<T>> &f) {
    auto F = f;
    cooley_tukey(n, F, [](int k) { return polar((T) 1, -M_PI / k); });
    return F;
}

template <typename T>
vector<complex<T>> ifft(int n, const vector<complex<T>> &F) {
    auto f = F;
    cooley_tukey(n, f, [](int k) { return polar((T) 1, -M_PI / k); });
    T n_inv = (T) 1 / n;
    for (auto &v : f) v *= n_inv;
    reverse(f.begin() + 1, f.end());
    return f;
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

    auto F = fft(n, dft);
    for (auto &v : F) v *= v;
    auto f = ifft(n, F);

    vector<T> c(m, 0.5);
    for (int i = 0; i < m; i++) c[i] *= f[i].imag();
    return c;
}

template <typename T>
vector<vector<T>> convolution_tree(int n, const vector<vector<T>> &leaves) {
    vector<vector<T>> ct(n);
    for (int i = 1; i < n; i <<= 1)
        for (int j = 0; j < n - i; j += i << 1) ct[j] = convolve(leaves[j], leaves[i + j]);
    return ct;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<double>> leaves(n);
    for (auto &poly : leaves) {
        double x;
        cin >> x;
        x /= 100;

        poly = {1 - x, x};
    }

    auto ct = convolution_tree(n, leaves);
    for (int k = 0; k <= n; k++) cout << fixed << setprecision(6) << ct[0][k] << " ";
}
