#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, p, t;
    cin >> n >> m >> p >> t;

    vector<vector<pair<int, long long>>> adj_list(n + 1);
    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, 1e18));
    for (int i = 1; i <= n; i++) dist[i][i] = 0;
    while (m--) {
        int a, b, l;
        cin >> a >> b >> l;

        adj_list[a].emplace_back(b, l);
        adj_list[b].emplace_back(a, l);
        dist[a][b] = dist[b][a] = l;
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            for (int k = 1; k <= n; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    int s = -1;
    for (auto [u, w] : adj_list[t])
        if (dist[p][t] == dist[p][u] + dist[u][t]) {
            s = u;
            break;
        }

    vector<bool> visited(n + 1, false);
    auto dfs1 = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (auto &[u, _] : adj_list[v])
            if (!((u == s && v == t) || (v == s && u == t)) && !visited[u]) self(self, u);
    };
    dfs1(dfs1, t);

    vector<vector<int>> farthest(n + 1);
    for (int i = 1; i <= n; i++) {
        if (adj_list[i].size() < 2) continue;

        if (i != t) {
            auto d = *max_element(dist[i].begin() + 1, dist[i].end());
            for (int j = 1; j <= n; j++)
                if (d == dist[i][j]) farthest[i].emplace_back(j);
        } else {
            auto d = 0LL;
            for (int j = 1; j <= n; j++)
                if (visited[j]) {
                    if (d == dist[i][j]) farthest[i].emplace_back(j);
                    else if (d < dist[i][j]) {
                        d = dist[i][j];
                        farthest[i].clear();
                        farthest[i].emplace_back(j);
                    }
                }
        }
    }

    auto d = dist[p][t];
    vector<int> state(n + 1, 0);
    vector<long long> dp(n + 1, 0);
    auto dfs2 = [&](auto &&self, int v) -> long long {
        if (state[v] == 2) return dp[v];

        state[v] = 1;
        auto longest = 0LL;
        for (int u : farthest[v])
            if (!state[u]) {
                auto l = self(self, u);
                if (l == 1e18) {
                    state[v] = 2;
                    return dp[v] = 1e18;
                }
                longest = max(longest, l + dist[v][u]);
            } else if (state[u] == 1) {
                state[v] = 2;
                return dp[v] = 1e18;
            } else longest = max(longest, dp[u] + dist[v][u]);

        state[v] = 2;
        return dp[v] = longest;
    };
    for (int i : farthest[t]) d = max(d, dfs2(dfs2, i) + dist[p][t] + dist[t][i]);

    if (d >= 1e18) cout << "impossible";
    else cout << d;
}
