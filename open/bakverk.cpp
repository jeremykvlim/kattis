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

    long long x, y;
    cin >> x >> y;

    if (x < y) {
        cout << -1;
        exit(0);
    }

    if (x == y) {
        cout << 1 << " " << 0;
        exit(0);
    }

    vector<tuple<int, int, long long, long long>> chain;
    unordered_set<tuple<long long, long long, int>, Hash> seen;
    int a = -1, b = -1;
    auto temp_x = x, temp_y = y;
    for (int nodes = 1; nodes < 29; nodes++) {
        for (a = 0; a < nodes; a++) {
            b = nodes - a - 1;
            chain.clear();
            seen.clear();
            auto dfs = [&](auto &&self, long long x, long long y, int remaining) -> bool {
                if (!y) return true;

                if (x == y) {
                    chain.emplace_back(1, 0, x, 0);
                    return true;
                }

                if (!remaining || x < y || !seen.emplace(make_tuple(x, y, remaining)).second) return false;

                for (int d = min((long long) a + b + 2, x); d > 1; d--) {
                    auto q = x / d;
                    int c = y / q;
                    if (c > a + 1 || c > d) continue;

                    auto r = y - q * c;
                    int w = d - c - !!r;
                    if (0 <= w && w <= b) {
                        chain.emplace_back(c, w, q, r);
                        if (!r || self(self, q, r, remaining - 1)) return true;
                        chain.pop_back();
                    }
                }
                return false;
            };
            if (dfs(dfs, temp_x, temp_y, 29 - a - b)) goto found;
        }
    }
    cout << -1;
    exit(0);

    found:;
    int s = chain.size(), n = s + a + b + 1;
    vector<pair<int, int>> edges;
    for (int u = 1; u <= a; u++) edges.emplace_back(u + s, n);
    for (int u = 1; u <= s; u++) {
        auto [c, w, q, r] = chain[u - 1];
        if (c) {
            for (int v = 1; v < c; v++) edges.emplace_back(u, v + s);
            edges.emplace_back(u, n);
        }
        for (int v = 1; v <= w; v++) edges.emplace_back(u, v + a + s);
        if (r) edges.emplace_back(u, u + 1);
    }

    cout << n << " " << edges.size() << "\n";
    for (auto [u, v] : edges) cout << u << " " << v << "\n";
}
