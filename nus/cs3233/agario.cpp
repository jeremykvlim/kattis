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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<array<int, 3>> edges(m);
    for (auto &[w, u, v] : edges) {
        cin >> u >> v;

        w = max(a[u], a[v]);
        if (a[u] < a[v]) swap(u, v);
    }
    sort(edges.begin(), edges.end());

    DisjointSets dsu(n + 1);
    vector<int> rep(n + 1);
    iota(rep.begin(), rep.end(), 0);

    vector<vector<int>> adj_list(n + 1);
    vector<bool> visited(n + 1, false);
    for (auto [w, u, v] : edges) {
        int u_set = rep[dsu.find(u)], v_set = rep[dsu.find(v)];

        auto [big, small] = dsu.unite(u, v);
        if (small != -1) {
            if (visited[v]) v = v_set;
            adj_list[u].emplace_back(v);
            visited[v] = true;
            rep[big] = u_set;
        }
    }

    vector<long long> size(a.begin(), a.end());

    auto dfs1 = [&](auto &&self, int v) -> void {
        for (int u : adj_list[v]) {
            self(self, u);
            size[v] += size[u];
        }
    };

    auto dfs2 = [&](auto &&self, int v, int prev = -1) -> void {
        if (prev != -1 && size[v] >= a[prev]) size[v] = size[prev];
        for (int u : adj_list[v]) self(self, u, v);
    };

    for (int i = 1; i <= n; i++)
        if (rep[dsu.find(i)] == i) {
            dfs1(dfs1, i);
            dfs2(dfs2, i);
            break;
        }

    for (int i = 1; i <= n; i++) cout << size[i] << " ";
}