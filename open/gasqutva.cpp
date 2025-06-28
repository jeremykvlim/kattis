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

    auto add = [&](const State &s1, const State &s2) -> State {
        return State{s1[0] + s2[0], s1[1] + s2[1]};
    };

    auto absorb = [&](vector<pair<State, int>> &states, int v) -> State {
        State accumulate = base();
        for (auto [s, _] : states) accumulate = add(accumulate, s);
        accumulate[0] += accumulate[1] * auxiliary[v];
        accumulate[1]++;
        return accumulate;
    };

    auto ascend = [&](State s, int w) -> State {
        return s;
    };

    reverse(order.begin(), order.end());
    vector<State> up(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        for (auto [u, w, i] : adj_list[v]) states.emplace_back(ascend(up[u], w), u);
        up[v] = absorb(states, v);
    }

    reverse(order.begin(), order.end());
    vector<State> down(n, base()), dp(n, base());
    for (int v : order) {
        vector<pair<State, int>> states;
        if (~parent_edge[v].first) states.emplace_back(ascend(down[v], parent_edge[v].second), -1);
        for (auto [u, w, i] : adj_list[v]) states.emplace_back(ascend(up[u], w), u);

        int m = states.size();
        dp[v] = absorb(states, v);

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
            down[u] = absorb(states, v);
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
