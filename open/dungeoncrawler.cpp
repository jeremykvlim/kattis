#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<pair<int, int>>> adj_list(n);
    auto total = 0LL;
    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u - 1].emplace_back(v - 1, w);
        adj_list[v - 1].emplace_back(u - 1, w);
        total += 2 * w;
    }

    vector<vector<array<int, 3>>> queries(n);
    for (int i = 0; i < q; i++) {
        int s, k, t;
        cin >> s >> k >> t;

        queries[s - 1].push_back({k - 1, t - 1, i});
    }

    vector<long long> time(q, LLONG_MAX);
    for (int s = 0; s < n; s++) {
        vector<int> in(n), out(n), prev(n, -1);
        vector<long long> dist(n);

        int count = 0;
        auto dfs = [&](auto &&self, int v) -> void {
            in[v] = count++;
            for (auto [u, w] : adj_list[v])
                if (u != prev[v]) {
                    prev[u] = v;
                    dist[u] = dist[v] + w;
                    self(self, u);
                }
            out[v] = count;
        };
        dfs(dfs, s);

        vector<long long> pref(n + 1), suff(n + 1);
        for (int i = 0; i < n; i++) {
            pref[in[i] + 1] = dist[i];
            suff[in[i]] = dist[i];
        }

        for (int i = 1; i <= n; i++) pref[i] = max(pref[i], pref[i - 1]);
        for (int i = n - 1; ~i; i--) suff[i] = max(suff[i], suff[i + 1]);

        auto ancestor = [&](int v, int u) {
            return in[v] <= in[u] && in[u] < out[v];
        };

        for (auto [k, t, i] : queries[s]) {
            if (ancestor(t, k)) {
                time[i] = -1;
                continue;
            }

            for (int l = in[k], r = l, j = k; l = in[j], r = out[j], j = prev[j]) {
                time[i] = min(time[i], total + 2 * dist[j] - max(pref[l], suff[r]));
                if (ancestor(j, t)) {
                    time[i] -= 2 * dist[j];
                    break;
                }
            }
        }
    }

    for (auto t : time) cout << (t == -1 ? "impossible" : to_string(t)) << "\n";
}
