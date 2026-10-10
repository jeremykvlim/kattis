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

    using State = pair<int, int>;
    auto base = [&]() -> State {
        return {0, -1};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        return {max(s1.first, s2.first), min(s1.second, s2.second)};
    };

    auto finalize = [&](vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        if (states.empty()) return t;

        if (states[0].first.second == -1) {
            sort(states.begin(), states.end(), [&](const auto &p1, const auto &p2) { return p1.first.first > p2.first.first; });
            for (int i = 0; i < states.size(); i++) {
                auto &[s, rank] = states[i].first;
                s += i + 1;
                rank = i;
                t.first = max(t.first, s);
            }
        } else
            for (auto [s, _] : states) t.first = max(t.first, s.first - (s.second > 0));
        return t;
    };

    auto climb = [&](State s, T w) -> State {
        return s;
    };

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) states.emplace_back(climb(up[u], w), u);
        up[v] = finalize(states, v);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base()), dp(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        if (parent[v] != -2) states.emplace_back(climb(down[v], parent_w[v]), -1);
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) states.emplace_back(climb(up[u], w), u);
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
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> bonsai(n);
    for (auto &nodule : bonsai) {
        int m;
        cin >> m;

        nodule.resize(m);
        for (int &v : nodule) cin >> v;
    }

    vector<tuple<int, int, int>> edges;
    vector<bool> visited(n, false);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        visited[v] = true;
        for (int u : bonsai[v])
            if (!visited[u]) {
                edges.push_back({v, u, 0});
                self(self, u);
            }
    };
    dfs(dfs);

    auto dp = rerooting_dp(n, edges);
    cout << min_element(dp.begin(), dp.end())->first;
}