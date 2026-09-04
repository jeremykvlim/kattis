#include <bits/stdc++.h>
using namespace std;

struct AntiMonopolyTree {
    vector<int> parent, size, weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, INT_MAX) {}

    pair<int, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        int max_w = INT_MIN, t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == INT_MAX) return {INT_MAX, -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    void upward_maintain(int v) {
        while (~parent[v]) {
            int p = parent[v];
            if (3 * size[v] <= 2 * size[p]) {
                v = p;
                continue;
            }

            size[p] -= size[v];
            parent[v] = parent[p];
            if (weight[v] < weight[p]) {
                size[v] += size[p];
                swap(weight[v], weight[p]);
                parent[p] = v;
            }
        }
    }

    int root(int v) {
        while (~parent[v]) v = parent[v];
        return v;
    }

    void cut(int v) {
        for (int p = parent[v]; ~p; p = parent[p]) size[p] -= size[v];
        parent[v] = -1;
        weight[v] = INT_MAX;
    }

    bool add(int u, int v, int w) {
        if (u == v) return false;

        upward_maintain(u);
        upward_maintain(v);

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == INT_MAX;
        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        }

        int du = 0, dv = 0;
        while (~u && ~v) {
            if (w >= weight[u]) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w >= weight[v]) {
                int p = parent[v];
                if (~p) size[p] += dv;
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                }

                du -= size[u];
                dv += size[u];
                size[v] += size[u];
                w = exchange(weight[u], w);
                u = exchange(parent[u], v);
                if (~u) size[u] += du;
            }
        }

        if (~v)
            for (v = parent[v]; ~v; v = parent[v]) size[v] += dv;
        return merged;
    }

    bool remove(int u, int v, int w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
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

    AntiMonopolyTree amt(n + 1);
    for (int i : mst) {
        auto [u, v, w] = edges[i];
        amt.add(u, v, special_nonspecial[i] ? -1 : w);
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < m; i++)
        if (special_nonspecial[i] && !in_mst[i]) {
            auto [u, v, w] = edges[i];
            auto [max_w, t] = amt.path_max(u, v);
            if (~t && ~max_w) pq.emplace(w - max_w, i);
        }

    while (W && !pq.empty()) {
        auto [d, i] = pq.top();
        pq.pop();

        auto [u, v, w] = edges[i];
        auto [max_w, t] = amt.path_max(u, v);
        if (~max_w && ~t) {
            if (w - max_w != d) {
                pq.emplace(w - max_w, i);
                continue;
            }
            total += d;
            amt.add(u, v, -1);
            W--;
        }
    }
    cout << (W ? -1 : total);
}