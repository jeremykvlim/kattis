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
    cooley_tukey(n, F, [](int k) { return polar((T) 1, (T) -M_PI / k); });
    return F;
}

template <typename T>
vector<complex<T>> ifft(int n, const vector<complex<T>> &F) {
    auto f = F;
    cooley_tukey(n, f, [](int k) { return polar((T) 1, (T) -M_PI / k); });
    T n_inv = (T) 1 / n;
    for (auto &v : f) v *= n_inv;
    reverse(f.begin() + 1, f.end());
    return f;
}

template <typename T>
vector<T> convolve(const vector<T> &a, const vector<T> &b) {
    int da = a.size(), db = b.size(), m = da + db - 1, n = bit_ceil((unsigned) m);
    if (n <= 256 || min(da, db) <= __lg(n)) {
        vector<T> x(da), y(db);
        for (int i = 0; i < da; i++) x[i] = a[i];
        for (int i = 0; i < db; i++) y[i] = b[i];
        if (da > db) {
            swap(x, y);
            swap(da, db);
        }

        vector<T> z(m, 0);
        for (int i = 0; i < m; i++) {
            int l = max(0, i - (db - 1)), r = min(i, da - 1) + 1;
            z[i] = inner_product(x.begin() + l, x.begin() + r, make_reverse_iterator(y.begin() + (i - l + 1)), (T) 0);
        }
        return z;
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
vector<T> poly_taylor_shift(const vector<T> &a, const T &c, const vector<T> &fact, const vector<T> &fact_inv) {
    int n = a.size();

    T pc = 1;
    vector<T> f(n), g(n);
    for (int i = 0; i < n; i++) {
        f[i] = a[n - 1 - i] * fact[n - 1 - i];
        g[i] = pc * fact_inv[i];
        pc *= c;
    }
    auto h = convolve(f, g);

    vector<T> b(n);
    for (int i = 0; i < n; i++) b[i] = fact_inv[i] * h[n - 1 - i];
    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n + 1, 0);
    for (int i = n; ~i; i--) cin >> x[i];

    int m;
    cin >> m;

    vector<int> y(m + 1, 0);
    for (int i = m; ~i; i--) cin >> y[i];

    int degree = max(n, m);
    x.resize(degree + 1, 0);
    y.resize(degree + 1, 0);
    if (!degree) {
        cout << "1 0 " << -x[0];
        exit(0);
    }

    int a, b;
    if (degree & 1) {
        a = x[degree];
        b = y[degree];
    } else {
        a = -y[degree];
        b =  x[degree];
    }

    if (!a && !b) {
        cout << "1 0 0";
        exit(0);
    }

    vector<double> odd(degree + 1), even(degree + 1);
    for (int i = 0; i <= degree; i++) {
        odd[i] = a * x[i] + b * y[i];
        even[i] = (-b) * x[i] + a * y[i];
    }
    auto &p = (degree & 1) ? odd : even;
    auto t0 = -p[degree - 1] / ((double) degree * p[degree]);

    vector<double> fact(degree + 1, 1), fact_inv(degree + 1, 1);
    auto prepare = [&]() {
        for (int i = 1; i <= degree; i++) {
            fact[i] = i * fact[i - 1];
            fact_inv[i] = fact_inv[i - 1] / i;
        }
    };
    prepare();

    odd = poly_taylor_shift(odd, t0, fact, fact_inv);
    even = poly_taylor_shift(even, t0, fact, fact_inv);

    for (int i = 2; i <= degree; i += 2)
        if (fabs(odd[i]) > 1e-6) {
            cout << "0 0 0";
            exit(0);
        }

    for (int i = 1; i <= degree; i += 2)
        if (fabs(even[i]) > 1e-6) {
            cout << "0 0 0";
            exit(0);
        }

    cout << fixed << setprecision(6) << a << " " << b << " " << -odd[0];
}
