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

    using State = array<int, 3>;
    auto base = [&]() -> State {
        return {-1, 0, INT_MAX};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        return {max(s1[0], s2[0]), max({s1[1], s2[1], s1[0] + s2[0]}), INT_MAX};
    };

    auto finalize = [&](const vector<pair<State, int>> &states) -> State {
        State t{0, 0, INT_MAX};
        for (auto [s, _] : states) t = merge(t, s);

        int p = -1;
        for (int i = 0; i < states.size(); i++)
            if (!~states[i].second) {
                p = i;
                break;
            }

        if (~p) {
            State temp{0, 0, INT_MAX};
            for (int i = 0; i < states.size(); i++)
                if (i != p) temp = merge(temp, states[i].first);

            int d1 = states[p].first[1], d2 = temp[1];
            t[2] = max({d1, d2, (d1 + 1) / 2 + (d2 + 1) / 2 + 1});
        }
        return t;
    };

    auto climb = [&](State s, T w) -> State {
        s[0]++;
        s[2] = INT_MAX;
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
                if (k + 1 < m) s.emplace_back(suff[k + 1], -1);
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

    vector<vector<int>> adj_list(n);
    vector<tuple<int, int, int>> edges(n - 1);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v;
        u--;
        v--;
        w = 0;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> prev(n, -1);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
    };
    prev[0] = -2;
    dfs(dfs);

    auto dp = rerooting_dp(n, edges);
    int flights = INT_MAX, cut_u = -1, cut_v = -1;
    for (int v = 1; v < n; v++)
        if (flights > dp[v][2]) {
            flights = dp[v][2];
            cut_u = v;
            cut_v = prev[v];
        }
    cout << flights << "\n" << cut_u + 1 << " " << cut_v + 1 << "\n";

    vector<int> dist(n);
    auto center = [&](int cut) {
        auto bfs = [&](int s) {
            fill(dist.begin(), dist.end(), -1);
            fill(prev.begin(), prev.end(), -1);
            dist[s] = 0;
            int furthest = s;
            queue<int> q;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                if (dist[furthest] < dist[v]) furthest = v;

                for (int u : adj_list[v]) {
                    if ((v == cut_u && u == cut_v) || (v == cut_v && u == cut_u) || ~dist[u]) continue;
                    dist[u] = dist[v] + 1;
                    prev[u] = v;
                    q.emplace(u);
                }
            }
            return furthest;
        };
        int farthest = bfs(bfs(cut)), diameter = dist[farthest];
        for (int i = 0; i < diameter / 2; i++) farthest = prev[farthest];
        return farthest;
    };
    cout << center(cut_u) + 1 << " " << center(cut_v) + 1;
}