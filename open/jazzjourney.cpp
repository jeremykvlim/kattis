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

    int d, n;
    cin >> d >> n;

    unordered_map<pair<int, int>, int, Hash> edges;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        if (i) edges[{a[i - 1], a[i]}]++;
    }

    int m;
    cin >> m;

    unordered_map<pair<int, int>, int, Hash> price_o, price_r;
    while (m--) {
        int sk, dk, pk;
        char tk;
        cin >> sk >> dk >> tk >> pk;

        auto p = make_pair(sk, dk);
        if (tk == 'O' && (!price_o.count(p) || pk < price_o[p])) price_o[p] = pk;
        if (tk == 'R' && (!price_r.count(p) || pk < price_r[p])) price_r[p] = pk;

        if (price_r.count(p) && (!price_o.count(p) || price_r[p] < price_o[p])) price_o[p] = price_r[p];
    }

    unordered_map<pair<int, int>, int, Hash> flights;
    auto cost = 0LL;
    for (int i = 1; i < n; i++) {
        auto f = make_pair(a[i - 1], a[i]), b = make_pair(a[i], a[i - 1]);

        auto one_way_trip = [&](auto p) -> void {
            edges[p]--;
            cost += price_o[p];
        };

        auto round_trip = [&]() -> void {
            edges[f]--;
            edges[b]--;
            flights[b]++;
            cost += price_r[f];
        };

        auto undo_round_trip = [&]() -> void {
            edges[b]++;
            edges[f]++;
            flights[f]--;
            cost -= price_r[b];
        };

        if (!flights[f]) {
            if (edges[b] && price_r[f] && price_r[f] < price_o[f] + price_o[b]) round_trip();
            else one_way_trip(f);
        } else {
            if (!edges[b] || !price_r[f] || price_r[f] > price_r[b]) flights[f]--;
            else {
                if (!edges[f] || price_r[b] > price_o[b] + price_o[f]) {
                    undo_round_trip();
                    one_way_trip(b);
                }
                round_trip();
            }
        }
    }

    cout << cost;
}
