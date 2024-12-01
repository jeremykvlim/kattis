#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int x, y;
        cin >> x >> y;

        adj_list[x].emplace_back(y);
        adj_list[y].emplace_back(x);
    }

    int count = 0;
    vector<int> order(n + 1), low(n + 1);
    vector<pair<int, int>> roads;
    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        for (int u : adj_list[v])
            if (u != prev) {
                if (!order[u]) {
                    self(self, u, v);
                    low[v] = min(low[v], low[u]);
                    if (low[u] > order[v]) {
                        cout << "NO\n";
                        exit(0);
                    }
                } else {
                    low[v] = min(low[v], order[u]);
                    if (order[u] > order[v]) continue;
                }
                roads.emplace_back(v, u);
            }
    };
    dfs(dfs);

    cout << "YES\n";
    for (auto [x, y] : roads) cout << x << " " << y << "\n";
}
