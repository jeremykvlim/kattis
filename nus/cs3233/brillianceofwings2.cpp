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
    vector<int> sets, prio;
    vector<pair<int, int>> weight;

    WeightedDisjointSets(int n) : sets(n), prio(n), weight(n, {INT_MAX, 0}) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]].first <= weight[v].first) sets[v] = sets[sets[v]];
        return sets[v];
    }

    int find(int v, int w = INT_MAX - 1) {
        while (weight[v].first <= w) v = compress(v);
        return v;
    }

    void detach(int v) {
        if (sets[v] == v) return;
        detach(sets[v]);
    }

    int attach(int v, int w = INT_MAX - 1) {
        while (weight[v].first <= w) v = sets[v];
        return v;
    }

    void link(int u, int v, pair<int, int> w) {
        detach(u);
        detach(v);
        while (u != v) {
            u = attach(u, w.first);
            v = attach(v, w.first);
            if (prio[u] < prio[v]) swap(u, v);
            swap(sets[v], u);
            swap(weight[v], w);
        }
        attach(u);
    }

    void cut(int v, int w) {
        while (sets[v] != v) {
            if (weight[v].first == w) {
                int u = v;
                while (sets[u] != u) u = sets[u];
                sets[v] = v;
                weight[v] = {INT_MAX, 0};
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
            if (weight[u].first > weight[v].first) swap(u, v);
            if (sets[u] == v) return u;
            u = sets[u];
        }
    }

    int unite(int u, int v, pair<int, int> w) {
        if (u != v) {
            int t = path_max(u, v);
            if (t == -1) {
                link(u, v, w);
                return -1;
            } else if (weight[t].first > w.first) {
                int i = weight[t].second;
                cut(t, weight[t].first);
                link(u, v, w);
                return i;
            }
        }
        return w.second;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_set<pair<int, int>, Hash> t1;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        t1.emplace(minmax(u - 1, v - 1));
    }

    vector<pair<int, int>> shared, t2;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        auto e = minmax(u - 1, v - 1);
        if (t1.count(e)) {
            t1.erase(e);
            shared.emplace_back(e);
        } else t2.emplace_back(e);
    }

    vector<pair<int, int>> edges(t1.begin(), t1.end());
    cout << edges.size() << "\n";

    WeightedDisjointSets wdsu(n);
    for (auto [u, v] : shared)
        if (wdsu.find(u) != wdsu.find(v)) wdsu.unite(u, v, {0, -1});

    for (int i = 0; i < edges.size(); i++) {
        auto [u, v] = edges[i];
        if (wdsu.find(u) != wdsu.find(v)) wdsu.unite(u, v, {i + 1, i});
    }

    for (auto [c, d] : t2) {
        int i = wdsu.unite(c, d, {0, -2});
        if (i != -2) {
            auto [a, b] = edges[i];
            cout << a + 1 << " " << b + 1 << " " << c + 1 << " " << d + 1 << "\n";
        }
    }
}
