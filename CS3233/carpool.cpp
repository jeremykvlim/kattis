#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<long long>> dist(n + 2, vector<long long>(n + 2, INT_MAX));
    while (m--) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        dist[u][v] = dist[v][u] = min(dist[v][u], w);
    }

    for (int i = 0; i < n + 2; i++) dist[i][i] = 0;
    for (int i = 0; i < n + 2; i++)
        for (int j = 0; j < n + 2; j++)
            for (int k = 0; k < n + 2; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    vector<long long> time(1 << n, INT_MAX);
    for (int p1 = 1; p1 < n + 1; p1++)
        for (int p2 = 1; p2 < n + 1; p2++)
            for (int p3 = 1; p3 < n + 1; p3++)
                for (int p4 = 1; p4 < n + 1; p4++)
                    for (int p5 = 1; p5 < n + 1; p5++)
                        time[((1 << p1) | (1 << p2) | (1 << p3) | (1 << p4) | (1 << p5)) >> 1] =
                                min(time[((1 << p1) | (1 << p2) | (1 << p3) | (1 << p4) | (1 << p5)) >> 1],
                                    dist[0][p1] + dist[p1][p2] + dist[p2][p3] + dist[p3][p4] + dist[p4][p5] + dist[p5][n + 1]
                                    + 5 * (1 + (p1 != p2) + (p2 != p3) + (p3 != p4) + (p4 != p5)));

    vector<long long> dp(1 << n, INT_MAX);
    dp[0] = 0;
    for (int i = 0; i < ceil((double) n / 5); i++) {
        auto temp = dp;
        for (int j = 0; j < 1 << n; j++)
            if (time[j] < INT_MAX)
                for (int k = (1 << n) - 1; (k &= ~j) >= 0; k--) temp[j | k] = min(max(dp[k], time[j]), temp[j | k]);
        dp = temp;
    }

    cout << dp[(1 << n) - 1];
}
