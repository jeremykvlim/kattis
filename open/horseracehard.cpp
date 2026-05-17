#pragma GCC optimize("Ofast,unroll-loops")
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

    vector<complex<T>> f(n);
    for (int i = 0; i < da; i++) f[i].real(a[i]);
    for (int i = 0; i < db; i++) f[i].imag(b[i]);

    fft(n, f);
    for (auto &v : f) v *= v;
    ifft(n, f);

    vector<T> c(m, 0.5);
    for (int i = 0; i < m; i++) c[i] *= f[i].imag();
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        int a, b;
        cin >> a >> b;

        if (a > b) cout << "1\n0";
        else cout << 0;
        exit(0);
    }

    vector<pair<int, int>> speeds(2 * n);
    for (int i = 0; i < 2 * n; i++) {
        int s;
        cin >> s;

        speeds[i] = {s, i};
    }
    sort(speeds.begin(), speeds.end());

    int size = 1e4, blocks = (2 * n + size - 1) / size;
    vector<int> a_pos(2 * n, -1), b_pos(2 * n, -1);
    vector<vector<int>> a_block(blocks), b_block(blocks);
    for (int rank = 0; rank < 2 * n; rank++) {
        auto [_, i] = speeds[rank];

        if (i < n) {
            a_pos[rank] = i;
            a_block[rank / size].emplace_back(i);
        } else {
            i -= n;
            b_pos[rank] = i;
            b_block[rank / size].emplace_back(i);
        }
    }

    vector<bool> flip(blocks);
    for (int b = 0; b < blocks; b++) {
        int b_l = b * size, b_r = min(2 * n, (b + 1) * size), total = 0;
        for (int i = b_l; i < b_r; i++)
            if (~a_pos[i]) total++;

        int count = 0, delta = 0;
        for (int i = b_l; i < b_r; i++) {
            if (~a_pos[i]) count++;
            if (~b_pos[i]) delta += total - 2 * count;
        }
        flip[b] = delta <= 0;
    }

    vector<int> c(n);
    vector<double> A(n), B(n);
    auto add = [&](const auto &a, const auto &b, int sgn) {
        if (a.empty() || b.empty()) return;

        fill(A.begin(), A.end(), 0);
        fill(B.begin(), B.end(), 0);
        for (int i : a) A[n - 1 - i] = 1;
        for (int i : b) B[i] = 1;

        auto C = convolve(A, B);
        for (int i = 0; i < n; i++) {
            int v = round(C[n - 1 + i]);
            if (i) v += round(C[i - 1]);
            c[i] += sgn * v;
        }
    };

    vector<int> a_active;
    for (int b = blocks - 1; ~b; b--) {
        if (flip[b]) add(a_active, b_block[b], 1);
        for (int i : a_block[b]) a_active.emplace_back(i);
    }

    a_active.clear();
    int base = 0;
    for (int b = 0; b < blocks; b++) {
        if (!flip[b]) {
            base += b_block[b].size();
            add(a_active, b_block[b], -1);
        }
        for (int i : a_block[b]) a_active.emplace_back(i);
    }

    for (int b = 0; b < blocks; b++) {
        int b_l = b * size, b_r = min(2 * n, (b + 1) * size);
        a_active.clear();
        if (flip[b])
            for (int i = b_r - 1; i >= b_l; i--) {
                if (~a_pos[i]) a_active.emplace_back(a_pos[i]);
                if (~b_pos[i]) {
                    int r = b_pos[i];
                    for (int l : a_active) {
                        int s = r - l;
                        c[s += (s >> 31) & n]++;
                    }
                }
            }
        else
            for (int i = b_l; i < b_r; i++) {
                if (~a_pos[i]) a_active.emplace_back(a_pos[i]);
                if (~b_pos[i]) {
                    int r = b_pos[i];
                    for (int l : a_active) {
                        int s = r - l;
                        c[s += (s >> 31) & n]--;
                    }
                }
            }
    }

    vector<int> shifts;
    for (int i = 0; i < n; i++) {
        c[i] += base;
        if (c[i] * 2 > n) shifts.emplace_back(i);
    }

    cout << shifts.size() << "\n";
    for (int s : shifts) cout << s << " ";
}
