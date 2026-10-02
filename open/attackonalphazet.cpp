#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;
    cin.ignore();

    int n = h * w;
    vector<vector<int>> adj_list(n);

    string s;
    getline(cin, s);

    for (int i = 0; i < h; i++) {
        string t;
        getline(cin, t);

        for (int j = 0; j < w; j++) {
            int v = i * w + j;
            if (i && s[2 * j + 1] == ' ') {
                adj_list[v].emplace_back(v - w);
                adj_list[v - w].emplace_back(v);
            }
            if (j + 1 < w && t[2 * j + 2] == ' ') {
                adj_list[v].emplace_back(v + 1);
                adj_list[v + 1].emplace_back(v);
            }
        }
        s = t;
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

    int m;
    cin >> m;

    auto d = 0LL;
    int u = -1;
    while (m--) {
        int x, y;
        cin >> x >> y;

        int v = (x - 1) * w + y - 1;
        if (~u) d += depth[u] + depth[v] - 2 * depth[lca(u, v)];
        u = v;
    }
    cout << d;
}
