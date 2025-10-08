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

struct WeightedDisjointSets {
    vector<int> sets, prio, weight;

    WeightedDisjointSets(int n) : sets(n), prio(n), weight(n, INT_MAX) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]] <= weight[v]) sets[v] = sets[sets[v]];
        return sets[v];
    }

    int find(int v, int w = INT_MAX - 1) {
        while (weight[v] <= w) v = compress(v);
        return v;
    }

    void detach(int v) {
        if (sets[v] == v) return;
        detach(sets[v]);
    }

    int attach(int v, int w = INT_MAX - 1) {
        while (weight[v] <= w) v = sets[v];
        return v;
    }

    void link(int u, int v, int w) {
        detach(u);
        detach(v);
        while (u != v) {
            u = attach(u, w);
            v = attach(v, w);
            if (prio[u] < prio[v]) swap(u, v);
            swap(sets[v], u);
            swap(weight[v], w);
        }
        attach(u);
    }

    void cut(int v, int w) {
        while (sets[v] != v) {
            if (weight[v] == w) {
                int u = v;
                while (sets[u] != u) u = sets[u];
                sets[v] = v;
                weight[v] = INT_MAX;
                return;
            }
            v = compress(v);
        }
    }

    void cut(int u, int v, int w) {
        cut(u, w);
        cut(v, w);
    }

    int path_max(int u, int v) {
        if (find(u) != find(v)) return -1;

        for (;;) {
            if (weight[u] > weight[v]) swap(u, v);
            if (sets[u] == v) return u;
            u = sets[u];
        }
    }

    int unite(int u, int v, int w) {
        if (u != v) {
            int t = path_max(u, v);
            if (t == -1) {
                link(u, v, w);
                return -1;
            } else if (weight[t] > w) {
                int temp = weight[t];
                cut(t, weight[t]);
                link(u, v, w);
                return temp;
            }
        }
        return w;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_set<pair<int, int>, Hash> t1, t2;
    auto read = [&](auto &t) {
        for (int _ = 0; _ < n - 1; _++) {
            int u, v;
            cin >> u >> v;

            t.emplace(minmax(u, v));
        }
    };
    read(t1);
    read(t2);

    WeightedDisjointSets wdsu(n);
    vector<pair<int, int>> edges;
    for (auto [u, v] : t1)
        if (t2.count({u, v})) wdsu.unite(u, v, -1);
        else {
            edges.emplace_back(u, v);
            wdsu.unite(u, v, edges.size() - 1);
        }

    cout << edges.size() << "\n";
    for (auto [c, d] : t2)
        if (!t1.count({c, d})) {
            int i = wdsu.unite(c, d, -1);
            auto [a, b] = edges[i];
            cout << a << " " << b << " " << c << " " << d << "\n";
        }
}
