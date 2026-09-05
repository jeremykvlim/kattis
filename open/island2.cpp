#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    vector<array<int, 4>> queries(q);
    for (auto &[r1, c1, r2, c2] : queries) {
        cin >> r1 >> c1 >> r2 >> c2;
        r1--;
        c1--;
        r2--;
        c2--;
    }

    vector<int> steps(q, 0);
    auto calc = [&]() {
        vector<vector<int>> id(n, vector<int>(n, -1)), adj_list;
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n;) {
                if (grid[r][c] == '.') {
                    c++;
                    continue;
                }

                adj_list.emplace_back();
                for (int v = adj_list.size() - 1, temp = c; c < n && grid[r][c] == '#'; c++) {
                    id[r][c] = v;
                    if (r && grid[r - 1][c] == '#' && (c == temp || grid[r - 1][c - 1] == '.')) {
                        int u = id[r - 1][c];
                        adj_list[v].emplace_back(u);
                        adj_list[u].emplace_back(v);
                    }
                }
            }

        int m = adj_list.size();

        auto lsb = [&](int x) {
            return x & -x;
        };

        vector<pair<int, int>> tour;
        vector<int> depth(m, 0), inlabel(m), ascendant(m, 0), head(m + 1);
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

        for (int i = 0; i < q; i++) {
            auto [r1, c1, r2, c2] = queries[i];
            int u = id[r1][c1], v = id[r2][c2];
            steps[i] += depth[u] + depth[v] - 2 * depth[lca(u, v)];
        }
    };
    calc();
    for (int r = 0; r < n; r++)
        for (int c = 0; c < r; c++) swap(grid[r][c], grid[c][r]);

    for (auto &[r1, c1, r2, c2] : queries) {
        swap(r1, c1);
        swap(r2, c2);
    }
    calc();
    for (int s : steps) cout << s << "\n";
}
