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

    using State = array<long long, 4>;
    auto base = [&]() -> State {
        return {0, 0, 0, 0};
    };

    auto add = [&](State s1, State s2) -> State {
        return State{s1[0] + s2[0], s1[1] + s2[1], s1[2] + s2[2], s1[3] + s2[3]};
    };

    auto absorb = [&](vector<pair<State, int>> &states) -> State {
        State accumulate = base();
        for (auto [s, _] : states) accumulate = add(accumulate, s);
        accumulate[0]++;
        return accumulate;
    };

    auto ascend = [&](State s, int w) -> State {
        s[3] += w * w * w * s[0];
        s[3] += 3 * w * w * s[1];
        s[3] += 3 * w * s[2];
        s[2] += 2 * s[1] * w + s[0] * w * w;
        s[1] += w * s[0];
        return s;
    };

    auto adjust = [&](vector<pair<State, int>> &states) -> void {};

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        for (auto [u, w, i] : adj_list[v]) states.emplace_back(ascend(up[u], w), u);
        adjust(states);
        up[v] = absorb(states);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base()), dp(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        if (~parent_edge[v].first) states.emplace_back(ascend(down[v], parent_edge[v].second), -1);
        for (auto [u, w, i] : adj_list[v]) states.emplace_back(ascend(up[u], w), u);
        adjust(states);
        dp[v] = absorb(states);

        int m = states.size();
        vector<State> pref(m), suff(m);
        for (int i = 0; i < m; i++) pref[i] = (!i ? states[i].first : add(pref[i - 1], states[i].first));
        for (int i = m - 1; ~i; i--) suff[i] = (i == m - 1 ? states[i].first : add(suff[i + 1], states[i].first));

        unordered_map<int, int> pos;
        for (int i = 0; i < m; i++) pos[states[i].second] = i;

        for (auto [u, w, i] : adj_list[v]) {
            states.clear();
            int k = pos[u];
            if (k > 0) states.emplace_back(pref[k - 1], -1);
            if (k + 1 < m) states.emplace_back(suff[k + 1], -1);
            down[u] = absorb(states);
        }
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
        cin >> u >> v >> w;
        u--;
        v--;
    }

    auto dist = LLONG_MAX;
    for (auto [size, lin, sq, cb] : rerooting_dp(n, edges)) dist = min(dist, cb);
    cout << dist * 2;
}
