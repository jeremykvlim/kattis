#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, h;
    cin >> n >> h;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    vector<int> hideouts;
    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        if (adj_list[v].size() == 1) hideouts.emplace_back(v);

        for (int u : adj_list[v])
            if (u != prev) self(self, u, v);
    };
    dfs(dfs, h);

    int m = (hideouts.size() + 1) / 2;
    cout << m << "\n";
    for (int i = 0; i < m; i++) cout << hideouts[i] << " " << hideouts[(i + m) % hideouts.size()] << "\n";
}
