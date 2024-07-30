#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<int, vector<int>> indices;
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        indices[i + x].emplace_back(i);
        for (int j : indices[i - x]) {
            adj_list[i].emplace_back(j);
            adj_list[j].emplace_back(i);
        }
    }

    vector<bool> visited(n, false);
    auto dfs = [&](auto &&self, int v = 0) -> int {
        visited[v] = true;

        int biggest = v;
        for (int u : adj_list[v])
            if (!visited[u]) biggest = max(biggest, self(self, u));

        return biggest;
    };
    
    cout << dfs(dfs);
}
