#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << 0;
        exit(0);
    }

    vector<vector<pair<int, int>>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u - 1].emplace_back(v - 1, w);
        adj_list[v - 1].emplace_back(u - 1, w);
    }

    vector<int> order, prev(n, -1);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        order.emplace_back(v);
        for (auto [u, w] : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
    };
    dfs(dfs);
    prev[0] = 0;

    auto add = [&](auto &s1, auto s2, int w) {
        s2[3] += w * w * w * s2[0];
        s2[3] += 3 * w * w * s2[1];
        s2[3] += 3 * w * s2[2];
        s2[2] += 2 * s2[1] * w + s2[0] * w * w;
        s2[1] += w * s2[0];
        for (int k = 0; k < 4; k++) s1[k] += s2[k];
    };

    vector<array<long long, 4>> dp1(n, {1, 0, 0, 0}), dp2(n);
    for (int i = n - 1; ~i; i--) {
        int v = order[i];
        for (auto [u, w] : adj_list[v])
            if (u != prev[v]) add(dp1[v], dp1[u], w);
    }

    for (int v : order) {
        dp1[prev[v]] = dp1[v];
        int m = adj_list[v].size();
        vector<array<long long, 4>> states(m, {1, 0, 0, 0});
        for (int b = __lg(m); ~b; b--) {
            for (int i = m - 1; ~i; i--) states[i] = states[i >> 1];
            for (int i = 0; i < m - (m & (!b)); i++) {
                auto [u, w] = adj_list[v][i];
                add(states[(i >> b) ^ 1], dp1[u], w);
            }
        }
        dp2[v] = states[0];
        auto [u, w] = adj_list[v][0];
        add(dp2[v], dp1[u], w);
        for (int i = 0; i < m; i++) dp1[adj_list[v][i].first] = states[i];
    }

    auto dist = LLONG_MAX;
    for (auto [size, lin, sq, cb] : dp2) dist = min(dist, cb);
    cout << dist * 2;
}
