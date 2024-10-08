#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string parties(n, 'A');
    vector<int> degree(n);
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < 5; i++) {
        int p;
        cin >> p;

        while (p--) {
            int k, l;
            cin >> k >> l;

            adj_list[k - 1].emplace_back(l - 1);
            adj_list[l - 1].emplace_back(k - 1);
            degree[k - 1]++;
            degree[l - 1]++;
        }
    }

    auto dfs = [&](auto &&self, int v) -> void {
        for (int u : adj_list[v]) {
            degree[v] += (parties[u] == parties[v]) ? -1 : 1;
            degree[u] += (parties[u] == parties[v]) ? -1 : 1;
        }

        parties[v] ^= 'A' ^ 'B';
        for (int u : adj_list[v])
            if (degree[u] > 2) self(self, u);
    };
    for (int i = 0; i < n; i++)
        if (degree[i] > 2) dfs(dfs, i);

    cout << parties;
}
