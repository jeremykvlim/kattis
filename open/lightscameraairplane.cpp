#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(n, m);
    if ((b - a) % g) return {0, -1};

    if (m > n) {
        swap(a, b);
        swap(n, m);
    }

    a %= n;
    b %= m;
    T lcm = n / g * m;

    n /= g;
    m /= g;
    auto [x, y] = bezout(n, m);
    T r = ((__int128) a * m * y + (__int128) b * n * x) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, T;
    cin >> n >> T;

    vector<int> k(n + 1);
    vector<vector<int>> c(n + 1);
    vector<int> parent(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> k[i];

        c[i].resize(k[i]);
        for (int j = 0; j < k[i]; j++) {
            cin >> c[i][j];

            parent[c[i][j]] = i;
        }
    }

    vector<int> t(n + 1, T + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b, ti;
        cin >> a >> b >> ti;

        t[parent[b] == a ? b : a] = ti;
    }

    vector<int> f(n + 1, 0);
    auto dfs = [&](auto &&self, int takeoffs, bool fixed = false, int i = 1, int a = 0, int m = 1) -> void {
        auto count = [&](int a, int m, int ti) {
            if (a != m) a = (a + m) % m;
            int k = 0;
            if (ti > a) k = (ti - a + m - 1) / m;

            if (takeoffs < a + k * m) return 0;
            return (takeoffs - (a + k * m)) / m + 1;
        };

        if (c[i].empty()) {
            if (takeoffs >= 1) f[i] += fixed ? 1 <= a && a <= takeoffs : count(a, m, 1);
            return;
        }

        int ki = c[i].size();
        for (int j = 0; j < ki; j++) {
            int ti = t[c[i][j]];

            if (takeoffs >= ti) {
                if (fixed) {
                    if (j == (a - 1 + ki) % ki) f[i] += ti <= a && a <= takeoffs;
                } else {
                    auto [r, lcm] = chinese_remainder_theorem(a, m, (j + 1) % ki, ki);
                    if (lcm != -1) f[i] += count(r, lcm, ti);
                }
            }
        }

        for (int j = 0; j < ki; j++) {
            int ti = min(takeoffs, t[c[i][j]] - 1);

            if (ti >= 1) {
                if (fixed) {
                    if (j == (a - 1 + ki) % ki) self(self, ti, true, c[i][j], a);
                } else {
                    auto [r, lcm] = chinese_remainder_theorem(a, m, (j + 1) % ki, ki);
                    if (lcm != -1) self(self, ti, lcm > T, c[i][j], r, lcm > T ? 1 : lcm);
                }
            }
        }
    };
    dfs(dfs, T);
    for (int i = 1; i <= n; i++) cout << f[i] << " ";
}
