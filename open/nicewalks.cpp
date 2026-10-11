#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, V, q;
    cin >> n >> V >> q;

    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1, i);
        adj_list[b - 1].emplace_back(a - 1, i);
    }

    vector<int> mask(n - 1, 0);
    while (V--) {
        char x;
        int r;
        cin >> x >> r;

        mask[r - 1] |= 1 << (x == 'P' ? 0 : x == 'C' ? 1 : 2);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(n, 0), inlabel(n), ascendant(n, 0), head(n + 1);
    vector<array<int, 3>> pref(n, {0, 0, 0});
    auto dfs = [&](auto &&self, int v = 0, int prev = 0) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();

        for (auto [u, i] : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                for (int j = 0; j < 3; j++) pref[u][j] = pref[v][j] + ((mask[i] >> j) & 1);
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

    while (q--) {
        int s, t;
        cin >> s >> t;

        int a = lca(s - 1, t - 1);
        bool nice = true;
        for (int i = 0; i < 3; i++) nice &= pref[s - 1][i] + pref[t - 1][i] > 2 * pref[a][i];
        cout << (nice ? "Y\n" : "N\n");
    }
}
