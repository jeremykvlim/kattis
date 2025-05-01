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

    int q, n, m;
    cin >> q >> n >> m;

    DisjointSets dsu(2 * q + 2);
    unordered_map<pair<int, int>, int, Hash> nodes;
    auto node = [&](int x, int y) {
        if (nodes.count({x, y})) return nodes[{x, y}];
        return nodes[{x, y}] = nodes.size() + 2;;
    };

    for (int i = 1; i <= q; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        if (make_pair(x1, y1) > make_pair(x2, y2)) {
            swap(x1, x2);
            swap(y1, y2);
        }

        if (x1 == x2) {
            if (!x1) dsu.unite(0, node(x1, y1));
            else if (x1 == n) dsu.unite(1, node(n - 1, y1));
            else dsu.unite(node(x1 - 1, y1), node(x1, y1));
        } else {
            if (!y1) dsu.unite(1, node(x1, y1));
            else if (y1 == m) dsu.unite(0, node(x1, m - 1));
            else dsu.unite(node(x1, y1 - 1), node(x1, y1));
        }

        if (dsu.find(0) == dsu.find(1)) {
            cout << i;
            exit(0);
        }
    }
    cout << -1;
}
