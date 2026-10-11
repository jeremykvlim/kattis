#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<vector<int>> adj_list(n);
        for (int _ = 0; _ < n - 1; _++) {
            int a, b;
            cin >> a >> b;

            adj_list[a - 1].emplace_back(b - 1);
            adj_list[b - 1].emplace_back(a - 1);
        }

        auto lsb = [&](int x) {
            return x & -x;
        };

        vector<pair<int, int>> tour;
        vector<int> depth(n, 0), inlabel(n), ascendant(n, 0), head(n + 1);
        auto dfs = [&](auto &&self, int v = 0, int prev = 0) -> void {
            tour.emplace_back(v, prev);
            inlabel[v] = tour.size();

            for (int u: adj_list[v])
                if (u != prev) {
                    depth[u] = depth[v] + 1;
                    self(self, u, v);
                    head[inlabel[u]] = v;
                    if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
                }
        };
        dfs(dfs);
        for (auto [v, p]: tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);

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

        bool b = true;
        for (int _ = 0, prev = -1; _ < n; _++) {
            int p;
            cin >> p;
            p--;

            if (~prev && b) {
                int w = lca(prev, p);
                if (depth[prev] + depth[p] - 2 * depth[w] > 3) b = false;
            }
            prev = p;
        }
        cout << b << "\n";
    }
}
