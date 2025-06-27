#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<T> &edges, const vector<int> &auxiliary) {
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

    using State = array<long long, 2>;
    auto base = [&]() -> State {
        return {0, 0};
    };

    auto absorb = [&](const State &s, int v) -> State {
        return State{s[0] + s[1] * auxiliary[v], s[1] + 1};
    };

    auto add = [&](const State &s1, const State &s2) -> State {
        return State{s1[0] + s2[0], s1[1] + s2[1]};
    };

    auto ascend = [&](State s, int w) -> State {
        return s;
    };

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        for (auto [u, w, i] : adj_list[v]) up[v] = add(up[v], ascend(up[u], w));
        up[v] = absorb(up[v], v);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base()), dp(n, base());
    for (int v : order) {
        int m = adj_list[v].size();
        vector<State> pref(m + 1, base()), suff(m + 1, base());
        if (~parent_edge[v].first) pref[0] = ascend(down[v], parent_edge[v].second);
        for (int i = 0; i < m; i++) {
            auto [u, w, _] = adj_list[v][i];
            pref[i + 1] = add(pref[i], ascend(up[u], w));
            suff[i] = ascend(up[u], w);
        }
        for (int i = m - 1; ~i; i--) suff[i] = add(suff[i], suff[i + 1]);

        dp[v] = absorb(pref[m], v);
        for (int i = 0; i < m; i++) {
            auto [u, w, _] = adj_list[v][i];
            down[u] = absorb(add(pref[i], suff[i + 1]), v);
        }
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<array<int, 3>> edges(n - 1);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v;
        u--;
        v--;
        w = 0;
    }

    int i = -1;
    auto extra = LLONG_MIN;
    auto dp = rerooting_dp(n, edges, a);
    for (int j = 0; j < n; j++) {
        auto e = dp[j][0] - (long long) a[j] * n;
        if (extra < e || (extra == e && i < j)) {
            extra = e;
            i = j;
        }
    }
    cout << i + 1;
}
