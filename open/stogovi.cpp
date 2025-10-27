#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> id(n + 1, 0), parent(n + 1, 0), op(n + 1, -1);
    vector<vector<int>> adj_list(n + 1);
    vector<array<int, 3>> queries;
    for (int i = 1; i <= n; i++) {
        char c;
        int v;
        cin >> c >> v;

        if (c == 'a') {
            id[i] = i;
            adj_list[parent[i] = id[v]].emplace_back(i);
        } else if (c == 'b') {
            id[i] = parent[id[v]];
            op[i] = id[v];
        } else {
            int w;
            cin >> w;

            id[i] = id[v];
            queries.push_back({id[v], id[w], i});
        }
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(n + 1), depth(n + 1, 0), anc_mask(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v = 0, int prev = 0) -> void {
        tour.emplace_back(v, prev);
        index[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
    };
    dfs(dfs);
    for (auto [v, p] : tour) anc_mask[v] = anc_mask[p] | lsb(index[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = index[u] ^ index[v]; above) {
            above = (anc_mask[u] & anc_mask[v]) & -bit_floor(above);
            if (unsigned below = anc_mask[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(index[u] & -below) | below];
            }
            if (unsigned below = anc_mask[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(index[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };
    for (auto [v, w, i] : queries) op[i] = depth[lca(v, w)];

    for (int i = 1; i <= n; i++)
        if (op[i] != -1) cout << op[i] << "\n";
}
