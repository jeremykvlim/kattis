#include <bits/stdc++.h>
using namespace std;

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
                int x = weight[t];
                cut(t, weight[t]);
                link(u, v, w);
                return x;
            }
        }
        return w;
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

    int n, m, k, W;
    cin >> n >> m >> k >> W;

    vector<bool> special(n + 1, false);
    while (k--) {
        int s;
        cin >> s;

        special[s] = true;
    }

    vector<array<int, 3>> edges(m);
    vector<bool> special_nonspecial(m);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;

        edges[i] = {a, b, c};
        special_nonspecial[i] = special[a] ^ special[b];
    }

    vector<int> order(m);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return special_nonspecial[i] != special_nonspecial[j] ? special_nonspecial[i] < special_nonspecial[j] : edges[i][2] < edges[j][2]; });

    DisjointSets dsu(n + 1);
    vector<int> mst;
    vector<bool> in_mst(m, false);
    auto total = 0LL;
    for (int i : order) {
        auto [u, v, w] = edges[i];
        if (dsu.unite(u, v)) {
            mst.emplace_back(i);
            in_mst[i] = true;
            total += w;
            W -= special_nonspecial[i];
            if (mst.size() == n - 1) break;
        }
    }

    if (mst.size() != n - 1 || W < 0) {
        cout << -1;
        exit(0);
    }

    if (!W) {
        cout << total;
        exit(0);
    }

    WeightedDisjointSets wdsu(n + 1);
    for (int i : mst) {
        auto [u, v, w] = edges[i];
        wdsu.unite(u, v, special_nonspecial[i] ? -1 : w);
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < m; i++)
        if (special_nonspecial[i] && !in_mst[i]) {
            auto [u, v, w] = edges[i];
            int t = wdsu.path_max(u, v);
            if (~t && ~wdsu.weight[t]) pq.emplace(w - wdsu.weight[t], i);
        }

    while (W && !pq.empty()) {
        auto [d, i] = pq.top();
        pq.pop();

        auto [u, v, w] = edges[i];
        int t = wdsu.path_max(u, v);
        if (~t && ~wdsu.weight[t]) {
            if (w - wdsu.weight[t] != d) {
                pq.push({w - wdsu.weight[t], i});
                continue;
            }
            total += w - wdsu.weight[t];
            wdsu.unite(u, v, -1);
            W--;
        }
    }
    cout << (W ? -1 : total);
}
