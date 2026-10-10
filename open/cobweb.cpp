#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<tuple<int, int, T>> &edges, const string &a, const string &b) {
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

    using State = array<int, 101>;
    auto base = [&]() -> State {
        return {};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        auto t = base();
        for (int i = 0; i <= b.size(); i++) t[i] = s1[i] + s2[i];
        return t;
    };

    auto finalize = [&](const vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        for (const auto &[st, _] : states) t = merge(t, st);

        int m = b.size();
        auto r = base();
        r[m] = t[m] + 1;
        for (int i = 0; i < m; i++)
            if (a[v] > b[i]) r[i] = r[m];
            else if (a[v] == b[i]) r[i] = t[i + 1] + (i == m - 1);
        return r;
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

    int n, m;
    string c, s;
    cin >> n >> m >> c >> s;

    vector<tuple<int, int, int>> edges;
    for (int _ = 0; _ < n - 1; _++) {
        int i, j;
        cin >> i >> j;

        edges.emplace_back(i - 1, j - 1, 1);
    }

    auto seqs = 0LL;
    for (auto &a : rerooting_dp(n, edges, c, s)) seqs += a[0];
    cout << seqs;
}