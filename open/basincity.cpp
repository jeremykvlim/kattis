#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n;
    cin >> k >> n;

    if (n >= 5 * k) {
        cout << "possible";
        exit(0);
    }

    vector<vector<int>> adj_list(n + 1);
    vector<vector<bool>> adj_matrix(n + 1, vector<bool>(n + 1, false));
    for (int v = 1; v <= n; v++) {
        int d;
        cin >> d;

        while (d--) {
            int u;
            cin >> u;

            adj_list[v].emplace_back(u);
            adj_matrix[v][u] = true;
        }
    }

    vector<bool> visited(n + 1, false);
    auto backtrack = [&](auto &&self, int k, int v = 1) -> bool {
        if (!k) return true;
        if (v > n) return false;

        if (visited[v]) return self(self, k, v + 1);
        visited[v] = true;

        auto neighbors = [&](int v) {
            auto neighbors = adj_list[v];
            erase_if(neighbors, [&](int u) { return visited[u]; });
            return neighbors;
        };
        auto neighbors_v = neighbors(v);
        for (int u : neighbors_v) visited[u] = true;
        if (self(self, k - 1, v + 1)) return true;
        for (int u : neighbors_v) visited[u] = false;

        for (int i = 0; i < neighbors_v.size(); i++)
            for (int j = i + 1; j < neighbors_v.size(); j++) {
                int x = neighbors_v[i], y = neighbors_v[j];
                if (!adj_matrix[x][y]) {
                    auto neighbors_x = neighbors(x), neighbors_y = neighbors(y);
                    visited[x] = visited[y] = true;
                    for (int u : neighbors_x) visited[u] = true;
                    for (int u : neighbors_y) visited[u] = true;
                    if (self(self, k - 2, v + 1)) return true;
                    for (int u : neighbors_y) visited[u] = false;
                    for (int u : neighbors_x) visited[u] = false;
                    visited[x] = visited[y] = false;
                }
            }
        return visited[v] = false;
    };
    cout << (backtrack(backtrack, k) ? "possible" : "impossible");
}
