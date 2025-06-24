#include <bits/stdc++.h>
using namespace std;

pair<vector<int>, vector<int>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1, 0);
    int count = 0;

    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u, v);
                low[v] = min(low[v], low[u]);
            } else if (u != prev) low[v] = min(low[v], order[u]);
    };
    dfs(dfs);

    return {order, low};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    auto [order, low] = tarjan(n, adj_list);

    vector<bool> visited(n + 1, false);
    vector<int> dp1(n + 1), dp2(n + 1), size(n + 1, 0);
    auto dfs = [&](auto &&self, int v = 1) -> void {
        visited[v] = true;

        int t = 0;
        for (int u : adj_list[v])
            if (!visited[u]) {
                self(self, u);

                if (low[u] == order[v]) {
                    dp1[v] = max(dp1[v] + size[u], size[v] + max(dp1[u], dp2[u] + 1));
                    size[v] += size[u];
                } else if (low[u] > order[v]) dp1[v] = max(dp1[v], size[v] + dp2[u] + 1);
                else {
                    t = u;
                    dp2[v] = dp2[u] + 1;
                }
            }
        size[0] = low[v] < order[v];
        dp2[v] = max(dp2[v] + size[v], dp1[v]);
        dp1[v] = max(dp1[v] + size[t], dp1[t]);
        size[v] += size[t] + 1;
    };
    dfs(dfs);
    cout << max(dp1[1], dp2[1]);
}
