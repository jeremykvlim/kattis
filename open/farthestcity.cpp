#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<tuple<int, int, T>> &edges, const vector<long long> &auxiliary) {
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

    using State = long long;
    auto base = [&]() -> State {
        return 0;
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        return max(s1, s2);
    };

    auto finalize = [&](vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        for (auto [s, _] : states) t = merge(t, s);
        return t;
    };

    auto climb = [&](State s, T w) -> State {
        return w < 0 ? auxiliary[-w - 1] : s + w;
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

    vector<int> degree(n, 0);
    vector<vector<pair<int, long long>>> adj_list(n);
    vector<tuple<int, int, long long>> edges(n);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;
        u--;
        v--;

        degree[u]++;
        degree[v]++;
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    queue<int> q;
    for (int v = 0; v < n; v++)
        if (degree[v] == 1) q.emplace(v);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        degree[v] = 0;
        for (auto [u, w] : adj_list[v])
            if (degree[u] && --degree[u] == 1) q.emplace(u);
    }

    vector<int> cycle;
    vector<long long> cycle_w;
    int s = find_if(degree.begin(), degree.end(), [&](int d) { return d > 0; }) - degree.begin(), curr = s, prev = -1;
    do {
        cycle.emplace_back(curr);
        for (auto [v, w] : adj_list[curr])
            if (degree[v] && v != prev) {
                cycle_w.emplace_back(w);
                prev = curr;
                curr = v;
                break;
            }
    } while (curr != s);

    int k = cycle.size();
    vector<int> root(n, -1);
    for (int i = 0; i < k; i++) root[cycle[i]] = i;

    vector<long long> dist(n, 0), height(k, 0);
    auto dfs = [&](auto &&self, int v) -> void {
        height[root[v]] = max(height[root[v]], dist[v]);
        for (auto [u, w] : adj_list[v])
            if (!~root[u]) {
                root[u] = root[v];
                dist[u] = dist[v] + w;
                self(self, u);
            }
    };
    for (int v : cycle) dfs(dfs, v);

    vector<long long> pref(2 * k + 1, 0);
    for (int i = 0; i < 2 * k; i++) pref[i + 1] = pref[i] + cycle_w[i % k];

    deque<int> dq1, dq2;
    vector<long long> farthest(k, 0);
    for (int l = 0, r = 1; l < k; l++) {
        for (int i = (l ? l + k - 1 : 1); i < l + k; i++) {
            while (!dq2.empty() && height[dq2.back() % k] - pref[dq2.back()] <= height[i % k] - pref[i]) dq2.pop_back();
            dq2.emplace_back(i);
        }
        while (!dq1.empty() && dq1.front() <= l) dq1.pop_front();

        r = max(r, l + 1);
        for (; r < l + k && 2 * (pref[r] - pref[l]) <= pref[k]; r++) {
            while (!dq1.empty() && height[dq1.back() % k] + pref[dq1.back()] <= height[r % k] + pref[r]) dq1.pop_back();
            dq1.emplace_back(r);
        }
        while (!dq2.empty() && dq2.front() < r) dq2.pop_front();

        if (!dq1.empty()) farthest[l] = max(farthest[l], height[dq1.front() % k] + pref[dq1.front()] - pref[l]);
        if (!dq2.empty()) farthest[l] = max(farthest[l], height[dq2.front() % k] - pref[dq2.front()] + pref[l] + pref[k]);
    }

    vector<tuple<int, int, long long>> temp;
    for (auto [u, v, w] : edges)
        if (!degree[u] || !degree[v]) temp.emplace_back(u, v, w);
    edges = temp;
    for (int i = 0; i < k; i++) edges.emplace_back(cycle[i], n, -i - 1);

    auto dp = rerooting_dp(n + 1, edges, farthest);
    for (int i = 0; i < n; i++) cout << dp[i] << " ";
}
