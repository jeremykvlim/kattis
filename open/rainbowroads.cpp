#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<tuple<int, int, T>> &edges, const vector<vector<T>> &auxiliary) {
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

    using State = pair<bool, int>;
    auto base = [&]() -> State {
        return {true, -1};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        return {s1.first && s2.first, -1};
    };

    auto finalize = [&](const vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        for (auto [s, _] : states) t = merge(t, s);
        t.second = v;
        return t;
    };

    auto climb = [&](State s, T w) -> State {
        if (s.first && binary_search(auxiliary[s.second].begin(), auxiliary[s.second].end(), w)) s.first = false;
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
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<int, int, int>> edges(n - 1);
    vector<vector<int>> colors(n);
    for (auto &[u, v, c] : edges) {
        cin >> u >> v >> c;
        u--;
        v--;

        colors[u].emplace_back(c);
        colors[v].emplace_back(c);
    }

    vector<vector<int>> bad(n);
    for (int v = 0; v < n; v++) {
        sort(colors[v].begin(), colors[v].end());
        for (int i = 1; i < colors[v].size(); i++)
            if (colors[v][i] == colors[v][i - 1] && (bad[v].empty() || bad[v].back() != colors[v][i])) bad[v].emplace_back(colors[v][i]);
    }
    auto dp = rerooting_dp(n, edges, bad);

    vector<int> good;
    for (int i = 0; i < n; i++)
        if (dp[i].first) good.emplace_back(i + 1);

    cout << good.size() << "\n";
    for (int v : good) cout << v << "\n";
}