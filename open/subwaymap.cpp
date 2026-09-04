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
    AntiMonopolyTree amt(n + 1);
    for (int i = 0; i < m; i++)
        if (c[i]) {
            amt.add(a[i], b[i], l[i]);
            if (unknown[i]) len[i] = 1;
        }

    for (int i = 0; i < m; i++)
        if (unknown[i]) {
            if (!c[i]) {
                auto [w, t] = amt.path_max(a[i], b[i]);
                if (~t) len[i] = w;
            }
            cout << len[i] << "\n";
        }
}