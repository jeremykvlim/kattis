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

    long long h, b;
    int m, n;
    cin >> h >> b >> m >> n;

    int gx = 0;
    vector<int> tricks(m);
    for (int &x : tricks) {
        cin >> x;

        gx = __gcd(gx, x);
    }

    vector<tuple<long long, int, int>> wizards(n);
    for (auto &[l, t, c] : wizards) cin >> l >> t >> c;

    auto reachable = [&](int g, bool s) -> bool {
        if (h % g) return false;
        if (b % g || !s) return true;
        return !((b < 0) ^ (b < h));
    };

    bool same = all_of(tricks.begin(), tricks.end(), [&](int x) { return x == gx; });
    if (reachable(gx, same)) {
        cout << 0;
        exit(0);
    }

    auto cost = LLONG_MAX;
    unordered_map<pair<int, bool>, long long, Hash> dist;
    dist[{gx, same}] = 0;
    priority_queue<tuple<long long, int, bool>, vector<tuple<long long, int, bool>>, greater<>> pq;
    pq.emplace(0, gx, same);
    while (!pq.empty()) {
        auto [d, g, start] = pq.top();
        pq.pop();

        if (dist[{g, start}] != d) continue;

        if (reachable(g, start)) cost = min(cost, d);

        for (auto [l, t, c] : wizards) {
            if (l == b || l % g || (!(b % g) && start && (b < 0) ^ (b < l)) || start && t == g) continue;

            int gt = __gcd(g, t);
            if (!dist.count({gt, false}) || dist[{gt, false}] > d + c) {
                dist[{gt, false}] = d + c;
                pq.emplace(d + c, gt, false);
            }
        }
    }

    cout << (cost == LLONG_MAX ? -1 : cost);
}
