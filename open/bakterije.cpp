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
    T g = __gcd(m, n);
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

    int n, m, k, tx, ty;
    cin >> n >> m >> k >> tx >> ty;
    tx--;
    ty--;

    vector<int> x(k), y(k), c(k);
    vector<vector<vector<int>>> boards(k, vector<vector<int>>(n, vector<int>(m)));
    for (int i = 0; i < k; i++) {
        char C;
        cin >> x[i] >> y[i] >> C;
        x[i]--;
        y[i]--;

        if (C == 'D') c[i] = 0;
        else if (C == 'R') c[i] = 1;
        else if (C == 'U') c[i] = 2;
        else c[i] = 3;

        for (auto &row : boards[i])
            for (int &cell : row) {
                char ch;
                cin >> ch;

                cell = ch - '0';
            }
    }

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    auto rotate = [&](int i) {
        c[i] -= boards[i][x[i]][y[i]];
        c[i] = (c[i] + 4) % 4;
        if (!(0 <= x[i] + dx[c[i]] && x[i] + dx[c[i]] < n && 0 <= y[i] + dy[c[i]] && y[i] + dy[c[i]] < m)) c[i] = (c[i] + 2) % 4;
        x[i] += dx[c[i]];
        y[i] += dy[c[i]];
    };

    int total = 4 * n * m;
    for (int t = 1; t < total; t++) {
        for (int i = 0; i < k; i++)
            if (x[i] != tx || y[i] != ty) goto next;
        cout << t - 1;
        exit(0);

        next:;
        for (int i = 0; i < k; i++) rotate(i);
    }

    vector<vector<array<long long, 2>>> cycles(k, vector<array<long long, 2>>(4, {-1, -1}));
    for (int i = 0; i < k; i++) {
        int sx = x[i], sy = y[i], sc = c[i], t = 0;
        for (; !t || x[i] != sx || y[i] != sy || c[i] != sc; t++) {
            if (x[i] == tx && y[i] == ty) cycles[i][c[i]][0] = t;

            rotate(i);
        }
        for (int d = 0; d < 4; d++) cycles[i][d][1] = t;
    }

    auto time = LLONG_MAX;
    auto dfs = [&](auto &&self, int i = 0, long long a = 0, long long n = 1) {
        if (i == k) {
            time = min(time, a + total);
            return;
        }

        for (int d = 0; d < 4; d++)
            if (~cycles[i][d][0]) {
                auto [r, lcm] = chinese_remainder_theorem(a, n, cycles[i][d][0], cycles[i][d][1]);
                if (lcm != -1) self(self, i + 1, r, lcm);
            }
    };
    dfs(dfs);

    cout << (time < LLONG_MAX ? time - 1 : -1);
}
