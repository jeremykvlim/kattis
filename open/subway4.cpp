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

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    unordered_set<pair<int, int>, Hash> plan;
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        if (a > b) swap(a, b);
        plan.emplace(a, b);
    }

    vector<int> depth(n), prev(n);
    auto dfs = [&](auto &&self, int v = 0, int p = -1) -> void {
        depth[v] = depth[p] + 1;
        prev[v] = p;
        for (int u : adj_list[v])
            if (u != p) self(self, u, v);
    };
    dfs(dfs);

    DisjointSets dsu(n);
    for (int a = 0; a < n; a++)
        for (int b : adj_list[a]) {
            if (a > b) continue;

            if (plan.count({a, b})) {
                plan.erase({a, b});

                if (prev[a] == b) dsu.unite(b, a);
                else dsu.unite(a, b);
            }
        }

    cout << plan.size() << "\n";
    for (auto [a2, b2] : plan) {
        int a1 = dsu.find(a2), b1 = dsu.find(b2);
        if (depth[a1] < depth[b1]) {
            swap(a1, b1);
            swap(a2, b2);
        }
        b1 = prev[a1];

        cout << a1 << " " << b1 << " " << a2 << " " << b2 << "\n";
        dsu.unite(b2, a2);
    }
}
