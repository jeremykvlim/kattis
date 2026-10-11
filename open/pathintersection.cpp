#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(n, 0), inlabel(n), ascendant(n, 0), head(n + 1);
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

    auto on_path = [&](int v, int u, int t) {
        int vu = lca(v, u), vt = lca(v, t), ut = lca(u, t);
        return vu == t || (vu == ut && vt == t) || (vu == vt && ut == t);
    };

    while (q--) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;

        int ab = lca(a - 1, b - 1);
        auto anc = [&](int v) {
            int x = ab, y = lca(a - 1, v), z = lca(b - 1, v);
            if (depth[x] < depth[y]) x = y;
            if (depth[x] < depth[z]) x = z;
            return x + 1;
        };
        int x = anc(c - 1), y = anc(d - 1);

        if (on_path(c - 1, d - 1, x - 1) && on_path(c - 1, d - 1, y - 1)) cout << x << " " << y << "\n";
        else cout << "-1\n";
    }
}