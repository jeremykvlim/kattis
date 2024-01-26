#include <bits/stdc++.h>
using namespace std;

vector<int> dfs(int v, int parent, vector<vector<int>> &adj_list) {
    vector<int> leaves;
    for (int u : adj_list[v]) {
        if (u == parent) continue;
        
        auto children = dfs(u, v, adj_list);
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

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        
        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    int root = find_if(adj_list.begin(), adj_list.end(), [&](auto neighbors) {return neighbors.size() > 1;}) - adj_list.begin();
    auto leaves = dfs(root, root, adj_list);

    int size = leaves.size();
    cout << (size + 1) / 2 << "\n";
    for (int i = 0; i < size / 2; i++) cout << leaves[i] << " " << leaves[i + size / 2] << "\n";
    if (size & 1) cout << leaves.front() << " " << leaves.back();
}
