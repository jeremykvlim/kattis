#include <bits/stdc++.h>
using namespace std;

struct WeightedDisjointSets {
    vector<int> sets, prio, weight;

    WeightedDisjointSets(int n) : sets(n), prio(n), weight(n, INT_MAX) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int &compress(int v) {
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
                int x = weight[t];
                cut(t, weight[t]);
                link(u, v, w);
                return x;
            }
        }
        return w;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    WeightedDisjointSets wdsu(n + 1);
    while (m--) {
        int u, v, t;
        cin >> u >> v >> t;

        wdsu.unite(u, v, -t);
    }

    int x = 0;
    while (q--) {
        int ap, bp, hp;
        cin >> ap >> bp >> hp;

        int a = ap ^ x, b = bp ^ x, h = hp ^ x;
        if (a == b) {
            cout << "Jebb\n";
            x++;
            continue;
        }

        if (wdsu.find(a) != wdsu.find(b)) {
            cout << "Neibb\n";
            continue;
        }

        if (h <= -wdsu.weight[wdsu.path_max(a, b)]) {
            cout << "Jebb\n";
            x++;
        } else cout << "Neibb\n";
    }
}
