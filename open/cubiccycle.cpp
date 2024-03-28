#include <bits/stdc++.h>
using namespace std;

int dfs(vector<vector<int>> &adj_list, vector<bool> &visited, int u) {
    if (count(visited.begin(), visited.end(), true) + 1 == visited.size()) return !adj_list[u][0] || !adj_list[u][1] || !adj_list[u][2];

    visited[u] = true;
    int degree = 0, cycles = 0;
    vector<int> neighbours(3, 0);
    for (int v : adj_list[u])
        if (!visited[v]) neighbours[degree++] = v;
    for (int i = 0; i < degree; i++) {
        if (degree == 2 && count_if(adj_list[neighbours[1 - i]].begin(), adj_list[neighbours[1 - i]].end(), [&](int u) {return u && visited[u];}) == 2) continue;
        cycles += dfs(adj_list, visited, neighbours[i]);
    }
    visited[u] = false;

    return cycles;
}

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
    cout << dfs(adj_list, visited, 0) / 2;
}
