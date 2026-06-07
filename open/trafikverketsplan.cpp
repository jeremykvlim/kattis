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
    vector<int> depth(n + 1, 0), up(n + 1, 0), down(n + 1, 0), inlabel(n + 1), ascendant(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();

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
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
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
