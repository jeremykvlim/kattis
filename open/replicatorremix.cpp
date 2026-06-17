#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> count(k + 1, 0);
    for (int _ = 0; _ < n; _++) {
        int t;
        cin >> t;

        count[t]++;
    }

    vector<int> c;
    for (int i = 1; i <= k; i++)
        if (count[i]) c.emplace_back(count[i]);

    sort(c.rbegin(), c.rend());
    if (c[0] == n) {
        cout << 0;
        exit(0);
    }
    if (!--c.back()) c.pop_back();

    int m = min(k, n - 1) + 1;
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(m, 0)));
    for (int y = 0; y < n; y++)
        for (int z = 0; z < m; z++) dp[0][y][z] = 1;

    for (int x = 1; x < n; x++)
        for (int y = 1; y < n; y++)
            for (int z = 1; z < m; z++) dp[x][y][z] = dp[x][y - 1][z] + (x >= y ? dp[x - y][y][z - 1] : 0);

    vector<int> pref_x(m), pref_y(m), pref_i(m);
    pref_x[0] = n - 1;
    vector<double> E(dp[n - 1][n - 1][m - 1]);
    auto dfs = [&](auto &&self, int X, int Y, int Z, int len = 0, int i = 0) -> void {
        if (!X) {
            if (len == 1) {
                E[i] = k;
                return;
            }

            int same = k - len;
            double sum = 0;
            for (int l = 0, r = 1; l < len; l = r++) {
                for (; r < len && pref_y[l] == pref_y[r]; r++);
                if (r == len && l + 1 == r) {
                    same++;
                    continue;
                }

                int j = pref_i[l];
                for (int p = l, x = pref_x[l], prev = l ? pref_y[l - 1] : n - 1, z = m - 1 - l; p < (len - (pref_y[len - 1] == 1)); p++) {
                    int y = min(x, prev), curr = pref_y[p];
                    if (p == l) curr++;
                    if (p == len - 1) curr--;
                    j += dp[x][y][z] - dp[x][curr][z];
                    x -= curr;
                    prev = curr;
                    z--;
                }
                sum += (r - l) * E[j];
            }
            E[i] = (k + sum) / (k - same);
            return;
        }
        if (!Z) return;

        for (int offset = 0, y = min(X, Y); y; y--) {
            self(self, pref_x[len + 1] = X - y, pref_y[len] = y, Z - 1, len + 1, pref_i[len + 1] = i + offset);
            offset += dp[X - y][y][Z - 1];
        }
    };
    dfs(dfs, n - 1, n - 1, m - 1);

    int i = 0;
    for (int x = n - 1, prev = n - 1; int curr : c) {
        int y = min(x, prev);
        i += dp[x][y][m - 1] - dp[x][curr][m - 1];
        x -= curr;
        prev = curr;
        m--;
    }
    cout << fixed << setprecision(5) << E[i];
}
