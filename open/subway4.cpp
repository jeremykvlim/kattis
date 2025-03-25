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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
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
