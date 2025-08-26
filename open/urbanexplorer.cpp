#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    vector<vector<int>> adj_list(m);
    for (int i = 0; i < m; i++) {
        int n;
        cin >> n;

        adj_list[i].resize(n);
        for (int &v : adj_list[i]) cin >> v;
    }

    queue<array<int, 3>> q;
    vector<vector<vector<int>>> degree(m, vector<vector<int>>(m, vector<int>(2, 0))), dp(m, vector<vector<int>>(m, vector<int>(2, 0)));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            degree[i][j][0] = adj_list[i].size();
            for (int v : adj_list[j]) degree[i][j][1] += !!v;
        }

        q.push({0, i, 0});
        q.push({0, i, 1});
        dp[0][i][0] = dp[0][i][1] = 1;

        if (i) {
            q.push({i, i, 0});
            q.push({i, i, 1});
            dp[i][i][0] = dp[i][i][1] = 2;
        }
    }

    while (!q.empty()) {
        auto [u, v, curr] = q.front();
        q.pop();

        vector<array<int, 3>> neighbors;
        if (!curr) {
            for (int j : adj_list[v])
                if (j) neighbors.push_back({u, j, 1});
        } else
            for (int i : adj_list[u]) neighbors.push_back({i, v, 0});

        for (auto [i, j, next] : neighbors)
            if (!dp[i][j][next]) {
                if ((dp[u][v][curr] == 1 && !next) || (dp[u][v][curr] == 2 && next)) {
                    dp[i][j][next] = dp[u][v][curr];
                    q.push({i, j, next});
                } else if (!--degree[i][j][next]) {
                    dp[i][j][next] = !next ? 2 : 1;
                    q.push({i, j, next});
                }
            }
    }

    if (!dp[1][2][0]) cout << "Stuck";
    else if (dp[1][2][0] == 1) cout << "Escaped";
    else cout << "Caught";
}
