#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_matrix(n + 1, vector<int>(n + 1, 2008));
    while (m--) {
        int a, b, y;
        cin >> a >> b >> y;

        adj_matrix[a][b] = adj_matrix[b][a] = y;
    }

    vector<int> group1, group2;
    auto dfs = [&](auto &&self, int y, int v = 1) {
        if (group1.size() > (2 * n) / 3 || group2.size() > (2 * n) / 3) return;

        if (v > n) {
            cout << y;
            exit(0);
        }

        auto check = [&](vector<int> group, bool first = true) {
            for (int u : group)
                if (first == (adj_matrix[v][u] >= y)) return false;
            return true;
        };

        if (check(group1)) {
            group1.emplace_back(v);
            self(self, y, v + 1);
            group1.pop_back();
        }

        if (check(group2, false)) {
            group2.emplace_back(v);
            self(self, y, v + 1);
            group2.pop_back();
        }
    };

    for (int y = 1948; y <= 2008; y++) dfs(dfs, y);
    cout << "Impossible";
}
