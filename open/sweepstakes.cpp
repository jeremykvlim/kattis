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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, t, Q;
    cin >> m >> n >> t >> Q;

    if (!Q) exit(0);

    vector<double> p(m), q(n);
    for (auto &pi : p) cin >> pi;
    for (auto &qj : q) cin >> qj;

    int size = 1 << 11;
    vector<complex<double>> roots(size), F_t(size, 1), F_row(size);
    for (int i = 0; i < size; i++) roots[i] = polar(1., -2 * M_PI * i / size);
    for (auto pi : p) {
        fill(F_row.begin(), F_row.end(), 1);
        for (auto qj : q) {
            auto p_mine = pi + qj, p_no_mine = 1 - p_mine;
            for (int i = 0; i < size; i++) F_row[i] *= p_mine * roots[i] + complex<double>(p_no_mine, 0);
        }
        for (int i = 0; i < size; i++) F_t[i] *= F_row[i];
    }
    auto f_t = ifft(size, F_t);

    vector<complex<double>> F_s(size), F_t_not_s(size);
    while (Q--) {
        int s;
        cin >> s;

        fill(F_s.begin(), F_s.end(), 1);
        for (int _ = 0; _ < s; _++) {
            int r, c;
            cin >> r >> c;

            auto p_mine = p[r - 1] + q[c - 1], p_no_mine = 1 - p_mine;
            for (int i = 0; i < size; i++) F_s[i] *= p_mine * roots[i] + complex<double>(p_no_mine, 0);
        }
        for (int i = 0; i < size; i++) F_t_not_s[i] = F_t[i] / F_s[i];

        auto f_s = ifft(size, F_s), f_t_not_s = ifft(size, F_t_not_s);
        for (int i = 0; i <= s; i++) cout << fixed << setprecision(6) << (!(0 <= t - i && t - i <= m * n - s) ? 0 : f_s[i % size].real() * f_t_not_s[(t - i) % size].real() / f_t[t % size].real()) << " ";
        cout << "\n";
    }
}
