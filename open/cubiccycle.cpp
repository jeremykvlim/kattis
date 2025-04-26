#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n, vector<int>(3, 0));
    vector<int> degree(n, 0);
    for (int i = 0; i < 3 * n / 2; i++) {
        int u, v;
        cin >> u >> v;

        adj_list[v][degree[v]++] = u;
        adj_list[u][degree[u]++] = v;
    }

    vector<bool> visited(n, false);
    auto dfs = [&](auto &&self, int v = 0) -> int {
        if (count(visited.begin(), visited.end(), true) + 1 == visited.size()) return !adj_list[v][0] || !adj_list[v][1] || !adj_list[v][2];

        visited[v] = true;
        int degree = 0, cycles = 0;
        vector<int> neighbors(3, 0);
        for (int u : adj_list[v])
            if (!visited[u]) neighbors[degree++] = u;

        for (int i = 0; i < degree; i++) {
            if (degree == 2 && count_if(adj_list[neighbors[1 - i]].begin(), adj_list[neighbors[1 - i]].end(), [&](int u) { return u && visited[u]; }) == 2) continue;
            cycles += self(self, neighbors[i]);
        }
        visited[v] = false;

        return cycles;
    };
    cout << dfs(dfs) / 2;
}
