#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<tuple<int, int, T>> &edges) {
    vector<vector<pair<int, T>>> adj_list(n);
    for (auto [u, v, w] : edges) {
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<int> order, parent(n, -1);
    vector<T> parent_w(n, 0);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        order.emplace_back(v);
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) {
                parent[u] = v;
                parent_w[u] = w;
                self(self, u);
            }
    };
    parent[0] = -2;
    dfs(dfs);

    using State = array<long long, 2>;
    auto base = [&]() -> State {
        return {0, 0};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        return {max(s1[0], s2[0]), max(max(s1[1], s2[1]), s1[0] + s2[0])};
    };

    auto finalize = [&](const vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        for (auto [s, _] : states) t = merge(t, s);
        return t;
    };

    auto climb = [&](State s, int w) -> State {
        s[0] += w;
        return s;
    };

    auto arrange = [&](vector<pair<State, int>> &states) -> void {};

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) states.emplace_back(climb(up[u], w), u);
        arrange(states);
        up[v] = finalize(states, v);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base()), dp(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        if (parent[v] != -2) states.emplace_back(climb(down[v], parent_w[v]), -1);
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) states.emplace_back(climb(up[u], w), u);
        arrange(states);
        dp[v] = finalize(states, v);

        int m = states.size();
        vector<State> pref(m), suff(m);
        for (int i = 0; i < m; i++) pref[i] = (!i ? states[i].first : merge(pref[i - 1], states[i].first));
        for (int i = m - 1; ~i; i--) suff[i] = (i == m - 1 ? states[i].first : merge(suff[i + 1], states[i].first));

        for (int k = 0; k < m; k++)
            if (~states[k].second) {
                vector<pair<State, int>> s;
                if (k) s.emplace_back(pref[k - 1], -1);
                if (k + 1 < m) s.emplace_back(suff[k + 1], -1);
                down[states[k].second] = finalize(s, v);
            }
    }
    return tuple{dp, up, down};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<tuple<int, int, int>> edges(n - 1);
        for (int i = 0; i < n - 1; i++) {
            int p, w;
            cin >> p >> w;

            edges[i] = {p - 1, i + 1, w};
        }

        auto [dp, up, down] = rerooting_dp(n, edges);
        auto diff = LLONG_MAX;
        for (int v = 1; v < n; v++) diff = min(diff, abs(up[v][1] - down[v][1]));
        cout << diff << "\n";
    }
}