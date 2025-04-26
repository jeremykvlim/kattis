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

    int c;
    while (cin >> c && c) {
        unordered_map<pair<array<int, 3>, array<int, 3>>, vector<int>, Hash> connections;
        vector<vector<int>> adj_list(c + 1);
        for (int i = 1; i <= c; i++) {
            int n;
            cin >> n;

            vector<array<int, 3>> corners(n);
            for (auto &[x, y, z] : corners) cin >> x >> y >> z;

            for (int j = 0; j < n; j++) {
                auto prev = corners[j], curr = corners[(j + 1) % n];

                for (int k : connections[{min(prev, curr), max(prev, curr)}]) {
                    adj_list[i].emplace_back(k);
                    adj_list[k].emplace_back(i);
                }

                connections[{min(prev, curr), max(prev, curr)}].emplace_back(i);
            }
        }

        int m;
        cin >> m;

        while (m--) {
            int a, b;
            cin >> a >> b;

            vector<int> dist(c + 1, INT_MAX);
            dist[a] = 0;
            queue<int> q;
            q.emplace(a);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (dist[u] > dist[v] + 1) {
                        dist[u] = dist[v] + 1;
                        q.emplace(u);
                    }
            }

            cout << dist[b] << "\n";
        }
    }
}
