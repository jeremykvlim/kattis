#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<vector<int>> dist(2, vector<int>(n, -1));
    auto dfs = [&](auto &&self, int v, int i) {
        if (dist[i][v] == -2) {
            cout << "infinity";
            exit(0);
        }

        if (dist[i][v] != -1) return dist[i][v];

        dist[i][v] = -2;
        int d = 0;
        for (int u : adj_list[v])
            if (a[v] == (a[u] ^ i)) d = max(d, self(self, u, i ^ 1));
        return dist[i][v] = d + 1;
    };

    array<array<int, 2>, 2> len{{{0, 0}, {0, 0}}};
    for (int v = 0; v < n; v++)
        for (int i = 0; i < 2; i++) len[i][a[v]] = max(len[i][a[v]], dfs(dfs, v, i));
    cout << min({len[0][0], len[0][1], len[1][0], len[1][1]}) + 1;
}
