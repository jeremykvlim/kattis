#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> perm(n + 1);
    for (int i = 1; i <= n; i++) cin >> perm[i];

    vector<vector<int>> s(n + 1, vector<int>(n + 1, 0));
    vector<int> a(m + 1), b(m + 1);
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i];

        s[a[i]][b[i]] = s[b[i]][a[i]] = i;
    }

    auto cmp = [&](auto p1, auto p2) {
        auto count = [&](vector<int> v) {
            int c = 0;
            for (int i = 1; i <= n; i++)
                if (v[i] != i) c += (s[i][v[i]] ? 1 : 2);

            return c;
        };

        return count(p1.first) + p1.second > count(p2.first) + p2.second;
    };

    unordered_map<vector<int>, int, Hash> indices;
    indices[perm] = 0;
    priority_queue<pair<vector<int>, int>, vector<pair<vector<int>, int>>, decltype(cmp)> pq(cmp);
    pq.emplace(perm, 0);
    while (!pq.empty()) {
        auto [v, d] = pq.top();
        pq.pop();

        if (is_sorted(v.begin(), v.end())) break;

        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) {
                auto u = v;

                if (s[i][j]) {
                    swap(u[i], u[j]);

                    if (indices.count(u)) continue;

                    indices[u] = s[i][j];
                    pq.emplace(u, d + 1);
                }
            }
    }

    auto dfs = [&](auto &&self, vector<int> v, int swaps = 0) -> void {
        if (!indices[v]) {
            cout << swaps;
            exit(0);
        }

        swap(v[a[indices[v]]], v[b[indices[v]]]);
        self(self, v, swaps + 1);
    };

    vector<int> ordered(n + 1);
    iota(ordered.begin(), ordered.end(), 0);
    dfs(dfs, ordered);
}
