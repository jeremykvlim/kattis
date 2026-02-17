#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> g(n, 0);
    for (int i = 2; i < n; i++) cin >> g[i];

    vector<unsigned long long> adj_masks(n, 0);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_masks[u - 1] |= 1LL << (v - 1);
        adj_masks[v - 1] |= 1LL << (u - 1);
    }

    vector<int> dist(n, 1e9);
    dist[1] = 0;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto mask = adj_masks[v]; mask; mask &= mask - 1) {
            int u = countr_zero(mask);
            if (dist[u] > dist[v] + 1) {
                dist[u] = dist[v] + 1;
                q.emplace(u);
            }
        }
    }

    vector<unsigned long long> adj_masks_dag(n, 0);
    for (int v = 0; v < n; v++)
        for (auto mask = adj_masks[v]; mask; mask &= mask - 1) {
            int u = countr_zero(mask);
            if (dist[u] + 1 == dist[v]) adj_masks_dag[v] |= 1LL << u;
        }

    vector<vector<int>> nodes(dist[0] + 1);
    for (int v = 0; v < n; v++)
        if (dist[v] != 1e9 && dist[v] <= dist[0]) nodes[dist[v]].emplace_back(v);

    vector<int> dp(n, 0);
    for (int d = 0; d <= dist[0]; d++)
        for (int v : nodes[d]) {
            for (auto mask = adj_masks_dag[v]; mask; mask &= mask - 1) {
                int u = countr_zero(mask);
                dp[v] = max(dp[v], dp[u]);
            }
            dp[v] += g[v];
        }

    int gold = 0;
    vector<bool> visited(n, false);
    auto dfs = [&](auto &&self, int sum, int v = 0, long long path_mask = 1) -> void {
        if (sum + dp[v] - g[v] <= gold) return;

        if (v == 1) {
            if (gold < sum) {
                fill(visited.begin(), visited.end(), false);
                fill(dist.begin(), dist.end(), 1e9);
                dist[0] = 0;
                for (int _ = 0; _ < n; _++) {
                    int a = -1;
                    for (int i = 0; i < n; i++)
                        if (!visited[i] && (a == -1 || dist[i] < dist[a])) a = i;
                    if (a == -1 || a == 1) break;

                    visited[a] = true;
                    for (auto mask = adj_masks[a]; mask; mask &= mask - 1) {
                        int b = countr_zero(mask);
                        dist[b] = min(dist[b], dist[a] + (((path_mask >> b) & 1) ? g[b] : 0));
                    }
                }
                gold = max(gold, sum - dist[1]);
            }
            return;
        }

        for (auto mask = adj_masks_dag[v]; mask; mask &= mask - 1) {
            int u = countr_zero(mask);
            self(self, sum + g[u], u, path_mask | (1LL << u));
        }
    };
    dfs(dfs, g[0]);
    cout << gold;
}
