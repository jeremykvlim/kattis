#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<T> &edges) {
    vector<vector<T>> adj_list(n);
    for (int i = 0; i < edges.size(); i++) {
        auto [u, v, w] = edges[i];
        adj_list[u].push_back({v, w, 2 * i});
        adj_list[v].push_back({u, w, 2 * i + 1});
    }

    vector<int> order;
    vector<pair<int, int>> parent_edge(n, {-1, -1});
    auto dfs = [&](auto &&self, int v = 0) -> void {
        order.emplace_back(v);
        for (auto [u, w, i] : adj_list[v]) {
            adj_list[u].erase(remove_if(adj_list[u].begin(), adj_list[u].end(), [i, v](const auto &e) { return e[0] == v && e[2] == (i ^ 1); }), adj_list[u].end());
            parent_edge[u] = {i ^ 1, w};
            self(self, u);
        }
    };
    dfs(dfs);

    using State = array<long long, 3>;
    auto base = [&]() -> State {
        return {0, 0, 0};
    };

    auto absorb = [&](State s) -> State {
        return State{s[0] + s[1] * s[1] - s[2], s[1] + 1, (s[1] + 1) * (s[1] + 1)};
    };

    auto add = [&](State s1, State s2) -> State {
        return State{s1[0] + s2[0], s1[1] + s2[1], s1[2] + s2[2]};
    };

    auto ascend = [&](State s, int w) -> State {
        s[0] += s[1] * w;
        return s;
    };

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        for (auto [u, w, i] : adj_list[v]) up[v] = add(up[v], ascend(up[u], w));
        up[v] = absorb(up[v]);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base());
    for (int v : order) {
        int m = adj_list[v].size();
        vector<State> pref(m + 1), suff(m + 1, base());
        if (~parent_edge[v].first) pref[0] = ascend(down[v], parent_edge[v].second);
        for (int i = 0; i < m; i++) {
            auto [u, w, _] = adj_list[v][i];
            pref[i + 1] = suff[i] = ascend(up[u], w);
        }
        for (int i = 1; i <= m; i++) pref[i] = add(pref[i - 1], pref[i]);
        for (int i = m - 1; ~i; i--) suff[i] = add(suff[i], suff[i + 1]);
        for (int i = 0; i < m; i++) {
            auto [u, w, _] = adj_list[v][i];
            down[u] = absorb(add(pref[i], suff[i + 1]));
        }
    }

    vector<State> dp(n, base());
    for (int v = 0; v < n; v++) {
        if (~parent_edge[v].first) dp[v] = ascend(down[v], parent_edge[v].second);
        for (auto [u, w, i] : adj_list[v]) dp[v] = add(dp[v], ascend(up[u], w));
        dp[v] = absorb(dp[v]);
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 3>> edges(n - 1);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v;
        u--;
        v--;
        w = 1;
    }

    auto dist = LLONG_MAX;
    for (auto [cost, size, sq] : rerooting_dp(n, edges)) dist = min(dist, cost);
    cout << dist;
}
