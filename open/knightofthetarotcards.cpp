#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

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

    int n, r0, c0, a0, b0, p0;
    cin >> n >> r0 >> c0 >> a0 >> b0 >> p0;

    if (!r0 && !c0) {
        cout << "0";
        exit(0);
    }

    vector<array<int, 5>> cards(n - 1);
    for (auto &[r, c, a, b, p] : cards) cin >> r >> c >> a >> b >> p;

    auto reduce = [&](int a, int b) -> pair<int, int> {
        int g = __gcd(a, b);

        if (((a / g) & 1) && ((b / g) & 1)) return {2, g};
        return {1, g};
    };

    auto s = reduce(a0, b0);
    gp_hash_table<pair<int, int>, long long, Hash> dist;
    auto relax = [&](long long d, auto v) -> bool {
        if (dist.find(v) == dist.end() || dist[v] > d) {
            dist[v] = d;
            return true;
        }
        return false;
    };
    relax(p0, s);
    for (auto [r, c, a, b, p] : cards)
        if (r == r0 && c == c0) relax(p, reduce(a, b));

    priority_queue<pair<long long, pair<int, int>>, vector<pair<long long, pair<int, int>>>, greater<>> pq;
    for (auto [v, d] : dist) pq.emplace(d, v);

    auto reachable = [&](pair<int, int> state, int r = 0, int c = 0) -> bool {
        auto [t, g] = state;
        if ((r - r0) % g || (c - c0) % g) return false;

        return t == 1 || !(((r - r0) / g) + ((c - c0) / g) & 1);
    };

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (dist[v] != d) continue;

        if (reachable(v)) {
            cout << d << "\n";
            exit(0);
        }

        auto [t1, g1] = v;
        for (auto [r, c, a, b, p] : cards) {
            if (!reachable(v, r, c)) continue;

            auto [t2, g2] = reduce(a, b);
            int g = __gcd(g1, g2);

            pair<int, int> u{t1, g};
            if (t1 != t2) u.first = !((t1 == 1 ? g1 : g2) % (2 * g)) ? 2 : 1;
            if (u == v) continue;

            if (relax(d + p, u)) pq.emplace(d + p, u);
        }
    }
    cout << -1;
}
