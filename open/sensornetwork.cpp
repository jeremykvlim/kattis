#include <bits/stdc++.h>
using namespace std;

struct WeightedDisjointSets {
    vector<int> sets, prio, weight;
    multiset<int, greater<>> weights;

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
        weights.emplace(w);
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
        weights.erase(weights.find(w));
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
    while (cin >> n && n) {
        int m;
        cin >> m;

        vector<array<int, 3>> edges(m);
        for (auto &[w, a, b] : edges) cin >> a >> b >> w;
        sort(edges.rbegin(), edges.rend());

        WeightedDisjointSets wdsu(n);
        int margin = INT_MAX, count = 0;
        for (auto [w, u, v] : edges) {
            if (wdsu.unite(u, v, w) == -1) count++;
            if (count == n - 1) margin = min(margin, *wdsu.weights.begin() - w);
        }
        cout << margin << "\n";
    }
}
