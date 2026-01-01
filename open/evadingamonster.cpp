#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n);
    vector<pair<int, int>> edges(2 * (n - 1));
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;

        int e1 = 2 * i, e2 = 2 * i + 1;
        edges[e1] = {u, v};
        edges[e2] = {v, u};
        adj_list[u].emplace_back(v, e1);
        adj_list[v].emplace_back(u, e2);
    }

    vector<int> a(m + 1);
    for (int i = 1; i <= m; i++) {
        cin >> a[i];
        a[i]--;
    }

    vector<int> parent(n, -1), parent_edge(n, -1);
    parent[0] = 0;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, e] : adj_list[v])
            if (parent[u] == -1) {
                parent[u] = v;
                parent_edge[u] = e ^ 1;
                q.emplace(u);
            }
    }

    vector<set<pair<int, int>>> st(n);
    vector<set<pair<int, int>>::iterator> it(2 * (n - 1));
    auto add = [&](int e, int w) {
        it[e] = st[edges[e].first].emplace(w, e).first;
    };

    auto remove = [&](int e) {
        st[edges[e].first].erase(it[e]);
    };
    for (int i = 0; i < 2 * (n - 1); i++) add(i, 0);

    vector<int> dp(n, 0);
    dp[a[m]] = 1e9;
    for (int i = m - 1; ~i; i--) {
        int u = a[i], v = a[i + 1], e1 = parent[u] == v ? parent_edge[u] : parent_edge[v] ^ 1, e2 = e1 ^ 1;

        remove(e1);
        while (!st[u].empty()) {
            auto [moves, e3] = *prev(st[u].end());

            if (moves <= dp[u] + 1) break;
            remove(e3);

            int t = edges[e3].second;
            if (moves != dp[t]) {
                add(e3, dp[t]);
                continue;
            }

            dp[t] = dp[u] + 1;
            add(e3, dp[u] + 1);
            remove(e3 ^ 1);
            add(e3 ^ 1, 1e9);
        }

        remove(e2);
        dp[u] = 1e9;
        while (!st[v].empty()) {
            auto [moves, e3] = *st[v].begin();

            int t = edges[e3].second;
            if (moves == dp[t]) {
                dp[v] = min(dp[v], moves + 1);
                break;
            }

            remove(e3);
            add(e3, dp[t]);
        }

        add(e1, dp[v]);
        add(e2, 1e9);
    }

    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> int {
        for (auto [u, e] : adj_list[v]) {
            if (u && u != prev) dp[v] = min(dp[v], self(self, u, v) + 1);
        }
        return dp[v];
    };
    dfs(dfs);
    cout << (dp[1] != 1e9 ? dp[1] : -1);
}
