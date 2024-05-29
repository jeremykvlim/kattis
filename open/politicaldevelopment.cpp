#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> adj_list1(n), adj_list2(n);
    vector<int> deg(n);
    for (int i = 0; i < n; i++) {
        int d;
        cin >> d;

        deg[i] = d;
        while (d--) {
            int j;
            cin >> j;

            adj_list1[i].emplace_back(j);
        }
    }

    for (int i = 0; i < n; i++)
        for (int j : adj_list1[i])
            if (deg[i] < deg[j] || (deg[i] == deg[j] && i < j)) adj_list2[i].emplace_back(j);

    int size = 0;
    vector<int> count(n, 0);
    auto dfs = [&](auto self, int v, int s = 1) -> void {
        size = max(size, s);
        for (int u : adj_list2[v]) count[u]++;
        for (int u : adj_list2[v])
            if (count[u] >= s) self(self, u, s + 1);
        for (int u : adj_list2[v]) count[u]--;
    };

    for (int i = 0; i < n; i++) dfs(dfs, i);

    cout << size;
}
