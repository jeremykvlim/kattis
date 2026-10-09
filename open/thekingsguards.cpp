#include <bits/stdc++.h>
using namespace std;

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

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

vector<array<int, 3>> kruskal(int n, vector<array<int, 3>> edges) {
    DisjointSets dsu(n);
    sort(edges.begin(), edges.end());

    vector<array<int, 3>> mst;
    for (auto e : edges) {
        auto [w, u, v] = e;
        if (~dsu.unite(u, v).second) mst.emplace_back(e);
    }

    return mst;
}

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, g;
    cin >> n >> r >> g;

    vector<array<int, 3>> e(r);
    for (auto &[w, u, v] : e) {
        cin >> u >> v >> w;
        u--;
        v--;
    }
    auto mst = kruskal(n, e);

    vector<bitset<300>> masks(n);
    vector<pair<int, int>> edges;
    for (int i = 0; i < g; i++) {
        int k;
        cin >> k;

        while (k--) {
            int v;
            cin >> v;

            masks[v - 1][i] = true;
            edges.emplace_back(i, v - 1);
        }
    }

    if (get<2>(hopcroft_karp(g, n, edges)) != g) {
        cout << -1;
        exit(0);
    }

    DisjointSets dsu(n);
    int count = 0, cost = 0;
    for (auto [w, u, v] : mst) {
        if (count == n - g) break;

        int u_set = dsu.find(u), v_set = dsu.find(v);
        if (u_set == v_set) continue;

        edges.clear();
        for (int t = 0; t < n; t++)
            if (dsu.find(t) == t && t != v_set) {
                auto temp = masks[t];
                if (t == u_set) temp |= masks[v_set];
                for (int i = 0; i < g; i++)
                    if (temp[i]) edges.emplace_back(i, t);
            }

        if (get<2>(hopcroft_karp(g, n, edges)) == g) {
            auto [big, small] = dsu.unite(u_set, v_set);
            masks[big] |= masks[small];
            count++;
            cost += w;
        }
    }
    cout << (count == n - g ? cost : -1);
}