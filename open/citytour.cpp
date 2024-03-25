#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 0; x < t; x++) {
        int n;
        cin >> n;

        vector<vector<int>> matrix(n, vector<int>(n, 0));
        vector<pair<int, int>> points(n - 3);
        for (auto &p : points) cin >> p.first >> p.second;

        for (int i = 0; i < 3; i++)
            for (int j = i + 1; j < 3; j++) matrix[i][j] = 1;

        for (int i = 0; i < points.size(); i++) matrix[points[i].first - 1][i + 3] = matrix[points[i].second - 1][i + 3] = 1;

        int y = 0;
        for (int i = n - 1; i >= 2; i--) {
            int a = -1, b = -1;
            for (int j = 0; j < i; j++)
                if (matrix[j][i]) {
                    if (a == -1) a = j;
                    else b = j;
                }

            y = max(y, matrix[a][b] + matrix[a][i] + matrix[b][i]);
            matrix[a][b] = max(matrix[a][b], matrix[a][i] + matrix[b][i]);
        }

        cout << "Case #" << (x + 1) << ": " << y << "\n";
    }
}
