#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<vector<array<int, 3>>> adj_list(n + 1);
        for (int i = 0; i < m; i++) {
            int x, y, c;
            cin >> x >> y >> c;

            adj_list[x].push_back({y, c, i});
        }

        vector<int> attraction(n + 1, -1), p, parent_edge(m, -1);
        vector<bool> visited(n + 1, false), used(m, false), path_edge(m, false);
        auto dfs = [&](auto &&self, int v, int prev_c, int prev_e) -> bool {
            if (visited[v] || attraction[v] == prev_c) return false;
            for (auto [u, c, i] : adj_list[v])
                if (c != prev_c && !used[i]) {
                    if (path_edge[i]) {
                        vector<int> path;
                        int k = prev_e;
                        while (k != i) {
                            path.emplace_back(k);
                            k = parent_edge[k];
                        }
                        reverse(path.begin(), path.end());
                        path.emplace_back(i);
                        p = path;
                        return true;
                    }
                    path_edge[i] = true;
                    parent_edge[i] = prev_e;
                    if (self(self, u, c, i)) return true;
                    path_edge[i] = false;
                    used[i] = true;
                }

            if (attraction[v] == -1) attraction[v] = prev_c;
            else if (attraction[v] != prev_c) visited[v] = true;
            return false;
        };

        for (int v = 1; v <= n && p.empty(); v++) dfs(dfs, v, 0, -1);

        if (p.empty()) cout << "NO\n";
        else {
            cout << "YES\n" << p.size() << " ";
            for (int i : p) cout << i + 1 << " ";
            cout << "\n";
        }
    }
}
