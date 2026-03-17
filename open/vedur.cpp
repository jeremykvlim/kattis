#include <bits/stdc++.h>
using namespace std;

struct WeightedDisjointSets {
    vector<int> sets, prio;
    vector<long long> weight;

    WeightedDisjointSets(int n) : sets(n), prio(n), weight(n, LLONG_MAX) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]] <= weight[v]) sets[v] = sets[sets[v]];
        return sets[v];
    }

    int find(int v, long long w = LLONG_MAX - 1) {
        while (weight[v] <= w) v = compress(v);
        return v;
    }

    void detach(int v) {
        if (sets[v] == v) return;
        detach(sets[v]);
    }

    int attach(int v, long long w = LLONG_MAX - 1) {
        while (weight[v] <= w) v = sets[v];
        return v;
    }

    void link(int u, int v, long long w) {
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

    void cut(int v, long long w) {
        while (sets[v] != v) {
            if (weight[v] == w) {
                for (int u = v; u != sets[u]; u = sets[u]);
                sets[v] = v;
                weight[v] = LLONG_MAX;
                return;
            }
            v = compress(v);
        }
    }

    void cut(int u, int v, long long w) {
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

    int unite(int u, int v, long long w) {
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

    int n, m;
    cin >> n >> m;

    auto index = [&](int r, int c) {
        return (r - 1) * m + c;
    };

    int k = n * m;
    vector<long long> a(k + 1);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[index(i, j)];

    WeightedDisjointSets wdsu(k + 1);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            int u = index(i, j);
            if (i < n) {
                int v = index(i + 1, j);
                wdsu.unite(u, v, max(a[u], a[v]));
            }
            if (j < m) {
                int v = index(i, j + 1);
                wdsu.unite(u, v, max(a[u], a[v]));
            }
        }

    int q;
    cin >> q;

    while (q--) {
        int l1, d1, l2, d2;
        cin >> l1 >> d1 >> l2 >> d2;

        int u = index(l1, d1), v = index(l2, d2);
        if (u == v) cout << a[u] << "\n";
        else cout << wdsu.weight[wdsu.path_max(u, v)] << "\n";
    }
}
