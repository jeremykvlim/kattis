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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
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
