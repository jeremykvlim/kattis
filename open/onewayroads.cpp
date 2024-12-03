#include <bits/stdc++.h>
using namespace std;

vector<pair<int, int>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1,0);
    vector<pair<int, int>> non_bridges;
    int count = 0;
    
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
                non_bridges.emplace_back(v, u);
            }
    };
    dfs(dfs);
    
    return non_bridges;
}

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

    auto roads = tarjan(n, adj_list);

    cout << "YES\n";
    for (auto [x, y] : roads) cout << x << " " << y << "\n";
}
