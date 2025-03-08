#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &seed, const T &v) {
        seed ^= Hash{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <typename ... Ts>
    static size_t seed_value(const Ts & ... args) {
        size_t seed = 0;
        (combine(seed, args), ...);
        return seed;
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto & ... elems) {return seed_value(elems...);}, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t seed = 0;
            for (const auto &e : v) combine(seed, e);
            return seed;
        } else return hash<T>{}(v);
    }
};

struct DisjointSets {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    DisjointSets dsu(n);
    unordered_set<pair<int, int>, Hash> edges;
    while (m--) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;

        if (x > y) swap(x, y);
        edges.emplace(x, y);
        dsu.unite(x, y);
    }

    vector<vector<int>> members(n);
    for (int i = 0; i < n; i++) members[dsu.find(i)].emplace_back(i);

    int ways = 0;
    for (int i = 0; i < n; i++) {
        if (members[i].size() != k) continue;

        for (int x : members[i])
            for (int y : members[i]) {
                if (x == y) continue;
                if (x > y) swap(x, y);
                if (!edges.count({x, y})) goto next;
            }

        ways++;
        next:;
    }
    cout << ways;
}
