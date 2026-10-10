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

    using State = array<long long, 5>;
    auto base = [&]() -> State {
        return {-1, -1, -1, -1, -1};
    };

    auto merge = [&](const State &s1, const State &s2) -> State {
        if (!~s1[0]) return s2;
        if (!~s2[0]) return s1;

        auto s3 = s1;
        if (s3[1] < s2[1]) {
            s3[1] = s2[1];
            s3[3] = s2[3];
            s3[4] = s2[4];
        }
        if (s3[1] < s1[0] + s2[0]) {
            s3[1] = s1[0] + s2[0];
            s3[3] = s1[2];
            s3[4] = s2[2];
        }
        if (s3[0] < s2[0]) {
            s3[0] = s2[0];
            s3[2] = s2[2];
        }
        return s3;
    };

    auto finalize = [&](const vector<pair<State, int>> &states, int v) -> State {
        auto t = base();
        for (auto [s, _] : states) t = merge(t, s);
        t = merge(t, {0, 0, v, v, v});
        return t;
    };

    auto climb = [&](State s, T w) -> State {
        s[0] += w;
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
    return tuple{dp, up, down};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<tuple<int, int, long long>> edges;
    vector<vector<pair<int, long long>>> adj_list(n);
    auto total = 0LL;
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        u--;
        v--;

        edges.emplace_back(u, v, w);
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
        total += 2 * w;
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> child(n, 0), depth(n, 0), in(n), inlabel(n), ascendant(n, 0), head(n + 1);
    vector<long long> dist(n);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 0, int prev = 0) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();
        in[v] = count++;

        for (auto [u, w] : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                dist[u] = dist[v] + w;
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);

    for (auto [v, p] : tour) {
        ascendant[v] = ascendant[p] | lsb(inlabel[v]);
        if (v && inlabel[v] == inlabel[p]) child[p] = v;
    }

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    vector<int> base, offset(n + 1), pos(n);
    for (auto [v, p] : tour)
        if (!v || inlabel[v] != inlabel[p]) {
            offset[inlabel[v]] = base.size();
            for (int u = v; ~u; u = child[u]) {
                base.emplace_back(u);
                pos[u] = base.size() - 1;
            }
        }

    auto climb = [&](int v, int d) {
        while (d) {
            if (d <= pos[v] - offset[inlabel[v]]) return base[pos[v] - d];
            d -= pos[v] - offset[inlabel[v]] + 1;
            v = head[inlabel[v]];
        }
        return v;
    };

    auto [dp, up, down] = rerooting_dp(n, edges);
    while (q--) {
        int s, k, t;
        cin >> s >> k >> t;
        s--;
        k--;
        t--;

        int x = lca(s, k), y = lca(s, t), z = lca(k, t);
        auto s_to_k = dist[s] + dist[k] - 2 * dist[x], s_to_t = dist[s] + dist[t] - 2 * dist[y], k_to_t = dist[k] + dist[t] - 2 * dist[z];
        if (s_to_k == s_to_t + k_to_t) {
            cout << "impossible\n";
            continue;
        }

        auto eccentricity = [&](int v, const auto &d) {
            return max(dist[d[3]] + dist[v] - 2 * dist[lca(d[3], v)], dist[d[4]] + dist[v] - 2 * dist[lca(d[4], v)]);
        };

        if (s_to_t == s_to_k + k_to_t) {
            cout << total - eccentricity(s, dp[0]) << "\n";
            continue;
        }

        int a = x ^ y ^ z;
        bool inside = a == x || a == z;
        int v = inside ? climb(k, depth[k] - depth[a] - 1) : a;
        cout << total - max(eccentricity(s, inside ? down[v] : up[v]), s_to_t - k_to_t + eccentricity(k, inside ? up[v] : down[v])) << "\n";
    }
}
