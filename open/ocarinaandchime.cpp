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
void fft(int n, vector<complex<T>> &f) {
    cooley_tukey(n, f, [](int k) { return polar((T) 1, (T) -M_PI / k); });
}

template <typename T>
void ifft(int n, vector<complex<T>> &f) {
    fft(n, f);
    T n_inv = (T) 1 / n;
    for (auto &v : f) v *= n_inv;
    reverse(f.begin() + 1, f.end());
}

template <typename T>
vector<complex<T>> convolve(const vector<complex<T>> &a, const vector<complex<T>> &b) {
    int da = a.size(), db = b.size(), m = da + db - 1, n = bit_ceil((unsigned) m);
    if (n <= 256 || min(da, db) <= __lg(n)) {
        auto x = a, y = b;
        if (da > db) {
            swap(x, y);
            swap(da, db);
        }

        vector<complex<T>> z(m);
        for (int i = 0; i < m; i++) {
            int l = max(0, i - (db - 1)), r = min(i, da - 1) + 1;
            z[i] = inner_product(x.begin() + l, x.begin() + r, make_reverse_iterator(y.begin() + (i - l + 1)), complex<T>());
        }
        return z;
    }

    vector<complex<T>> f_a(n);
    for (int i = 0; i < da; i++) f_a[i] = a[i];
    fft(n, f_a);

    if (a == b)
        for (int i = 0; i < n; i++) f_a[i] *= f_a[i];
    else {
        vector<complex<T>> f_b(n);
        for (int i = 0; i < db; i++) f_b[i] = b[i];
        fft(n, f_b);
        for (int i = 0; i < n; i++) f_a[i] *= f_b[i];
    }

    ifft(n, f_a);

    f_a.resize(m);
    return f_a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, k;
    cin >> m >> n >> k;

    vector<int> A(m), B(n);
    for (int &ai : A) cin >> ai;
    for (int &bi : B) cin >> bi;

    vector<bool> invalid(m - n + 1, false);
    vector<complex<double>> a(m), b(n), roots;
    for (int d = 2; d <= k; d++)
        if (!(k % d)) {
            roots.resize(d);
            for (int i = 0; i < d; i++) roots[i] = polar(1., 2 * M_PI * i / d);
            for (int i = 0; i < m; i++) a[i] = roots[A[i] % d];
            for (int i = 0; i < n; i++) b[n - 1 - i] = roots[B[i] % d];

            auto c = convolve(a, b);
            for (int i = 0; i <= m - n; i++)
                if (norm(c[i + n - 1]) > 1e-8) invalid[i] = true;
        }

    cout << count_if(invalid.begin(), invalid.end(), [](auto b) { return !b; }) << "\n";
    for (int i = 0; i <= m - n; i++)
        if (!invalid[i]) cout << i + 1 << " ";
}
