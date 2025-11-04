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

    using State = int;
    auto base = [&]() -> State {
        return 0;
    };

    auto add = [&](const State &s1, const State &s2) -> State {
        return max(s1, s2);
    };

    auto absorb = [&](vector<pair<State, int>> &states) -> State {
        auto accumulate = base();
        for (auto [s, _] : states) accumulate = add(accumulate, s);
        return accumulate;
    };

    auto ascend = [&](State s, int w) -> State {
        return s;
    };

    auto adjust = [&](vector<pair<State, int>> &states) -> void {
        sort(states.rbegin(), states.rend());
        for (int i = 0; i < states.size(); i++) states[i].first += i + 1;
    };

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
            if (k) states.emplace_back(pref[k - 1], -1);
            if (k + 1 < m) states.emplace_back(suff[k + 1] - 1, -1);
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

    vector<vector<int>> bonsai(n);
    for (auto &nodule : bonsai) {
        int m;
        cin >> m;

        nodule.resize(m);
        for (int &v : nodule) cin >> v;
    }

    vector<array<int, 3>> edges;
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
