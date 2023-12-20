#include <bits/stdc++.h>
using namespace std;

vector<int> dfs(int v, int parent, vector<vector<int>> &adj) {
    vector<int> leaves;
    for (int u : adj[v]) {
        if (u == parent) continue;
        auto children = dfs(u, v, adj);
        leaves.insert(leaves.end(), children.begin(), children.end());
    }
    
    if (leaves.empty()) leaves.emplace_back(v + 1);
    return leaves;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u - 1].emplace_back(v - 1);
        adj[v - 1].emplace_back(u - 1);
    }

    int root = distance(adj.begin(), find_if(adj.begin(), adj.end(), [&](auto neighbors) {return neighbors.size() > 1;}));
    auto leaves = dfs(root, root, adj);

    int size = leaves.size();
    cout << (size + 1) / 2 << "\n";
    for (int i = 0; i < size / 2; i++) cout << leaves[i] << " " << leaves[i + size / 2] << "\n";
    if (size & 1) cout << leaves[0] << " " << leaves.back();
}
