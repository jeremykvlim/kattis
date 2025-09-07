#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<array<int, 3>>> adj_list(n + 1);
    unordered_map<pair<int, int>, pair<int, long long>, Hash> state;
    while (m--) {
        int u, v, t, l;
        cin >> u >> v >> t >> l;

        while (l--) {
            int c;
            cin >> c;

            adj_list[v].push_back({u, c, t});
            state[{u, c}].first++;
        }
    }

    vector<long long> dist(n + 1, 1e18), dp(n + 1, 1e18);
    dist[n] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, n);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist[v]) continue;

        dp[v] = d;
        for (auto [u, c, w] : adj_list[v]) {
            auto &[count, du] = state[{u, c}];
            du = max(du, d + w);
            if (!--count)
                if (dist[u] > du) {
                    dist[u] = du;
                    pq.emplace(du, u);
                }
        }
    }

    if (dp[1] == 1e18) cout << "impossible";
    else cout << dp[1];
}
