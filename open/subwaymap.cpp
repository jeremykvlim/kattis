#include <bits/stdc++.h>
using namespace std;

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

    int n, m;
    cin >> n >> m;

    vector<int> a(m), b(m), l(m, 1), c(m);
    vector<bool> unknown(m, false);
    for (int i = 0; i < m; i++) {
        string li;
        cin >> a[i] >> b[i] >> li >> c[i];

        if (li != "?") l[i] = stoi(li);
        else unknown[i] = true;
    }

    vector<int> len(m, 0);
    WeightedDisjointSets wdsu(n + 1);
    for (int i = 0; i < m; i++)
        if (c[i]) {
            int w = l[i];
            wdsu.unite(a[i], b[i], {w, i});
            if (unknown[i]) len[i] = 1;
        }

    for (int i = 0; i < m; i++)
        if (unknown[i]) {
            if (!c[i]) {
                int t = wdsu.path_max(a[i], b[i]);
                if (~t) len[i] = wdsu.weight[t].first;
            }
            cout << len[i] << "\n";
        }
}
