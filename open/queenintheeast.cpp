#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c, p, q, tc = 1;
    while (cin >> c >> p >> q && c && p && q) {
        vector<int> f(c + 1);
        for (int i = 1; i <= c; i++) cin >> f[i];

        vector<vector<int>> dist(c + 1, vector<int>(c + 1, 1e9));
        for (int i = 1; i <= c; i++) dist[i][i] = 0;

        while (p--) {
            int c1, c2, d;
            cin >> c1 >> c2 >> d;

            dist[c1][c2] = min(dist[c1][c2], d);
            dist[c2][c1] = min(dist[c2][c1], d);
        }

        vector<pair<int, int>> queries(q);
        for (auto &[c1, c2] : queries) cin >> c1 >> c2;

        vector<int> order(c);
        iota(order.begin(), order.end(), 1);
        sort(order.begin(), order.end(), [&](int i, int j) { return f[i] < f[j]; });

        vector<vector<int>> dp(c + 1, vector<int>(c + 1, 1e9));
        for (int ci = 0; ci < c; ci++) {
            int i = order[ci];
            for (int j = 1; j <= c; j++)
                for (int k = 1; k <= c; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

            for (int j = 1; j <= c; j++)
                for (int k = 1; k <= c; k++)
                    if (dist[j][k] != 1e9) dp[j][k] = min(dp[j][k], dist[j][k] + max({f[i], f[j], f[k]}));
        }

        cout << tc++ << "\n";
        for (auto [c1, c2] : queries)
            if (dp[c1][c2] == 1e9) cout << "-1\n";
            else cout << dp[c1][c2] << "\n";
        cout << "\n";
    }
}
