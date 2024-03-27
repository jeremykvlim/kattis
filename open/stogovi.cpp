#include <bits/stdc++.h>
using namespace std;

int lca(int u, int v, vector<int> parent, vector<int> depth, vector<vector<int>> sparse) {
    if (depth[u] > depth[v]) swap(u, v);
    for (int i = 0, d = depth[v] - depth[u]; i < sparse.size(); i++)
        if (d & (1 << i)) v = sparse[i][v];

    if (u == v) return u;

    for (int i = sparse.size() - 1; ~i; i--)
        if (sparse[i][u] != sparse[i][v]) {
            u = sparse[i][u];
            v = sparse[i][v];
        }

    return parent[u];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> top(n + 1, 0), id(n + 1, 0), parent(n + 1, 0), depth(n + 1, 0);
    vector<vector<int>> sparse(__lg(n), vector<int>(n + 1, 0));
    for (int i = 1; i <= n; i++) {
        char c;
        int v;
        cin >> c >> v;

        if (c == 'a') {
            top[i] = id[i] = i;
            sparse[0][i] = parent[i] = id[v];
            depth[i] = depth[id[v]] + 1;
            for (int j = 1; j < sparse.size(); j++) sparse[j][i] = sparse[j - 1][sparse[j - 1][i]];
        } else if (c == 'b') {
            id[i] = parent[id[v]];
            cout << top[id[v]] << "\n";
        } else {
            int w;
            cin >> w;

            id[i] = id[v];
            cout << depth[lca(id[i], id[w], parent, depth, sparse)] << "\n";
        }
    }
}
