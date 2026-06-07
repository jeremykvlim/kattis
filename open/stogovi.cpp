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
    vector<int> depth(n + 1, 0), inlabel(n + 1), ascendant(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v = 0, int prev = 0) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);
    for (auto [v, p] : tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };
    for (auto [v, w, i] : queries) op[i] = depth[lca(v, w)];

    for (int i = 1; i <= n; i++)
        if (op[i] != -1) cout << op[i] << "\n";
}
