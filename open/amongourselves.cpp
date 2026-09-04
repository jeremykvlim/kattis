#include <bits/stdc++.h>
using namespace std;

struct AntiMonopolyTree {
    vector<int> parent, size, weight, XOR;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, INT_MAX), XOR(n, 0) {}

    int path_xor(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        int x = 0;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == INT_MAX) return -1;
            x ^= XOR[u];
            u = parent[u];
        }
        return x;
    }

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
            XOR[v] ^= XOR[p];
            if (weight[v] < weight[p]) {
                size[v] += size[p];
                swap(weight[v], weight[p]);
                parent[p] = v;
                XOR[p] ^= XOR[v];
            }
            if (!~parent[v]) XOR[v] = 0;
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
        XOR[v] = 0;
    }

    bool add(int u, int v, int w, int x) {
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
                x ^= XOR[u];
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w >= weight[v]) {
                x ^= XOR[v];
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
                x ^= exchange(XOR[u], x);
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

    AntiMonopolyTree amt(n + 1);
    int tl = 2 * m, tr = 2 * m;
    for (int i = t; i < m; i++) {
        auto [u, v, x] = edges[i];
        amt.add(u, v, tl--, x);
    }

    for (int i = 0; i <= m - t; i++) {
        int XOR = amt.path_xor(0, k - 1);
        if (~XOR) {
            cout << i + 1 << (!XOR ? " crewmate" : " imposter");
            exit(0);
        }

        if (i == m - t) {
            cout << -1;
            exit(0);
        }

        auto [u, v, x] = edges[i];
        amt.add(u, v, tl--, x);

        auto [a, b, _] = edges[i + t];
        amt.remove(a, b, tr--);
    }
}