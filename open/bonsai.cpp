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

    using State = int;
    auto base = [&]() -> State {
        return 0;
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        return max(s1, s2);
    };

    auto finalize = [&](const vector<pair<State, int>> &states) -> State {
        auto t = base();
        for (auto [s, _] : states) t = merge(t, s);
        return t;
    };

    auto climb = [&](State s, int w) -> State {
        return s;
    };

    auto arrange = [&](vector<pair<State, int>> &states) -> void {
        sort(states.rbegin(), states.rend());
        for (int i = 0; i < states.size(); i++) states[i].first += i + 1;
    };

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) states.emplace_back(climb(up[u], w), u);
        arrange(states);
        up[v] = finalize(states);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base()), dp(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        if (parent[v] != -2) states.emplace_back(climb(down[v], parent_w[v]), -1);
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) states.emplace_back(climb(up[u], w), u);
        arrange(states);
        dp[v] = finalize(states);

        int m = states.size();
        vector<State> pref(m), suff(m);
        for (int i = 0; i < m; i++) pref[i] = (!i ? states[i].first : merge(pref[i - 1], states[i].first));
        for (int i = m - 1; ~i; i--) suff[i] = (i == m - 1 ? states[i].first : merge(suff[i + 1], states[i].first));

        for (int k = 0; k < m; k++)
            if (~states[k].second) {
                vector<pair<State, int>> s;
                if (k) s.emplace_back(pref[k - 1], -1);
                if (k + 1 < m) s.emplace_back(suff[k + 1] - 1, -1);
                down[states[k].second] = finalize(s);
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
    cout << *min_element(dp.begin(), dp.end());
}