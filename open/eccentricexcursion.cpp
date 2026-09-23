#include <bits/stdc++.h>
using namespace std;

template <typename T>
auto rerooting_dp(int n, const vector<tuple<int, int, T>> &edges, const vector<bool> &auxiliary) {
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

    using State = array<int, 3>;
    auto base = [&]() -> State {
        return {0, INT_MIN, INT_MIN};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        State t{s1[0] + s2[0], INT_MIN, INT_MIN};
        for (int x : {s1[1], s1[2], s2[1], s2[2]})
            if (t[1] < x) {
                t[2] = t[1];
                t[1] = x;
            } else t[2] = max(t[2], x);
        return t;
    };

    auto finalize = [&](const vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        for (auto [s, _] : states) t = merge(t, s);
        if (!auxiliary[v]) return {t[0], INT_MIN, INT_MIN};
        return {t[0] + max(0, t[1]) + max(0, t[2]), t[0] + max(0, t[1]), INT_MIN};
    };

    auto climb = [&](State s, T w) -> State {
        if (s[1] == INT_MIN) return {s[0], INT_MIN, INT_MIN};
        return {s[0], 1 + s[1] - s[0], INT_MIN};
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

    int n, k;
    cin >> n >> k;

    vector<tuple<int, int, int>> edges(n - 1);
    vector<vector<bool>> adj_matrix(n, vector<bool>(n));
    for (auto &[a, b, w] : edges) {
        cin >> a >> b;
        a--;
        b--;
        w = 0;

        adj_matrix[a][b] = adj_matrix[b][a] = true;
    }

    vector<bool> active(n, true);
    for (int m = n - 1, u = -1, flights = 0; ~m; m--) {
        int count = 0;
        vector<int> degree(n, 0), match(n, -1);
        for (auto [a, b, w] : edges)
            if (active[a] && active[b]) {
                degree[a]++;
                degree[b]++;
                match[a] = b;
                match[b] = a;
                count++;
            }

        int x = -1, y = -1, z = -1;
        for (int a = 0; a < n; a++)
            if (active[a]) {
                if (!~x || degree[a] > degree[x]) {
                    z = y;
                    y = x;
                    x = a;
                } else if (!~y || degree[a] > degree[y]) {
                    z = y;
                    y = a;
                } else if (!~z || degree[a] > degree[z]) z = a;
            }

        int v = -1;
        auto dp = rerooting_dp(n, edges, active);
        for (int a = 0; a < n; a++)
            if (active[a]) {
                int l = m - dp[a][1], r = m - (m && degree[a] == m), f = k - flights - (~u && !adj_matrix[u][a]);
                if (m >= 2 && count - degree[a] == m - 1) {
                    int b = degree[a] ? match[a] : -1, d = ~b ? degree[b] - 1 : -1;
                    for (int t : {x, y, z})
                        if (t != a && t != b) {
                            d = max(d, degree[t]);
                            break;
                        }
                    r -= d == m - 1;
                }

                if (l <= f && f <= r) {
                    v = a;
                    break;
                }
            }

        if (!~v) {
            cout << -1;
            exit(0);
        }

        if (~u && !adj_matrix[u][v]) flights++;
        cout << v + 1 << " ";
        active[v] = false;
        u = v;
    }
}