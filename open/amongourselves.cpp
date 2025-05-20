#include <bits/stdc++.h>
using namespace std;

struct WeightedDisjointSets {
    vector<int> sets, prio, weight, XOR;

    WeightedDisjointSets(int n) : sets(n), prio(n), weight(n, INT_MAX), XOR(n, 0) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]] <= weight[v]) {
            XOR[v] ^= XOR[sets[v]];
            sets[v] = sets[sets[v]];
        }
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

    pair<int, int> attach(int v, int w = INT_MAX - 1) {
        int x = 0;
        while (weight[v] <= w) {
            x ^= XOR[v];
            v = sets[v];
        }
        return {v, x};
    }

    void link(int u, int v, int w, int x) {
        detach(u);
        detach(v);
        int y;
        while (u != v) {
            tie(u, y) = attach(u, w);
            x ^= y;
            tie(v, y) = attach(v, w);
            x ^= y;
            if (prio[u] < prio[v]) swap(u, v);
            swap(sets[v], u);
            swap(weight[v], w);
            swap(XOR[v], x);
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
                XOR[v] = 0;
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

    int path_xor(int u, int v) {
        int x = 0;
        while (sets[u] != u || sets[v] != v) {
            if (sets[u] != u) x ^= XOR[u];
            if (sets[v] != v) x ^= XOR[v];
            u = sets[u];
            v = sets[v];
        }
        return x;
    }

    int unite(int u, int v, int w, int x) {
        if (u != v) {
            int t = path_max(u, v);
            if (t == -1) {
                link(u, v, w, x);
                return -1;
            } else if (weight[t] > w) {
                int temp = weight[t];
                cut(t, weight[t]);
                link(u, v, w, x);
                return temp;
            }
        }
        return w;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, t;
    cin >> n >> m >> k >> t;

    vector<array<int, 3>> edges(m);
    for (auto &[a, b, s] : edges) {
        string S;
        cin >> a >> b >> S;
        a--;
        b--;

        s = S == "imposter";
    }

    WeightedDisjointSets dsu(n + 1);
    int tl = 2 * m, tr = 2 * m;
    for (int i = t; i < m; i++) {
        auto [u, v, x] = edges[i];
        dsu.unite(u, v, tl--, x);
    }

    for (int i = 0; i <= m - t; i++) {
        if (dsu.find(0) == dsu.find(k - 1)) {
            cout << i + 1 << (!dsu.path_xor(0, k - 1) ? " crewmate\n" : " imposter\n");
            exit(0);
        }

        if (i == m - t) {
            cout << -1;
            exit(0);
        }

        auto [u, v, x] = edges[i];
        dsu.unite(u, v, tl--, x);

        auto [a, b, _] = edges[i + t];
        dsu.cut(a, b, tr--);
    }
}
