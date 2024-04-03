#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int n;
        cin >> n;

        vector<pair<int, int>> points(n - 3);
        for (auto &[a, b] : points) cin >> a >> b;

        vector<vector<int>> adj_matrix(n, vector<int>(n, 0));
        for (int i = 0; i < 3; i++)
            for (int j = i + 1; j < 3; j++) adj_matrix[i][j] = 1;

        for (int i = 0; i < points.size(); i++)
            adj_matrix[points[i].first - 1][i + 3] = adj_matrix[points[i].second - 1][i + 3] = 1;

        int y = 0;
        for (int i = n - 1; i >= 2; i--) {
            int a = -1, b = -1;
            for (int j = 0; j < i; j++)
                if (adj_matrix[j][i]) {
                    if (a == -1) a = j;
                    else b = j;
                }

            y = max(y, adj_matrix[a][b] + adj_matrix[a][i] + adj_matrix[b][i]);
            adj_matrix[a][b] = max(adj_matrix[a][b], adj_matrix[a][i] + adj_matrix[b][i]);
        }

        cout << "Case #" << x << ": " << y << "\n";
    }
}
