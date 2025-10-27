#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<pair<int, bool>>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b, true);
        adj_list[b].emplace_back(a, false);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(n + 1), depth(n + 1, 0), up(n + 1, 0), down(n + 1, 0), anc_mask(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
        tour.emplace_back(v, prev);
        index[v] = tour.size();

        for (auto [u, d] : adj_list[v]) {
            if (u != prev) {
                depth[u] = depth[v] + 1;
                if (d) {
                    down[u] = down[v] + 1;
                    up[u] = up[v];
                } else {
                    up[u] = up[v] + 1;
                    down[u] = down[v];
                }
                self(self, u, v);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
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

    int Q;
    cin >> Q;

    while (Q--) {
        int h, w;
        cin >> h >> w;

        int a = lca(h, w);
        if (up[h] - up[a] != depth[h] - depth[a] || down[w] - down[a] != depth[w] - depth[a]) {
            cout << "nej\n";
            continue;
        }
        cout << "ja\n";
    }
}
