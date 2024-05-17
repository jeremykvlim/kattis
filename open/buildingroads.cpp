#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;

    double len = 0;
    vector<vector<double>> dist(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++) {
            dist[i][j] = dist[j][i] = hypot(coords[i].first - coords[j].first, coords[i].second - coords[j].second);
            len += dist[i][j];
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    vector<vector<int>> indices(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        iota(indices[i].begin(), indices[i].end(), 0);
        sort(indices[i].begin(), indices[i].end(), [&](int j, int k) {return dist[i][j] < dist[i][k];});
    }
    for (int i = 0; i < n; i++) len = min(len, dist[i][indices[i][n - 1]] * 2);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            int longest = indices[i][n - 1];
            for (int k = n - 1; ~k; k--)
                if (dist[j][indices[i][k]] > dist[j][longest]) {
                    len = min(len, dist[i][indices[i][k]] + dist[i][j] + dist[j][longest]);
                    longest = indices[i][k];
                }
        }

    cout << fixed << setprecision(3) << len;
}
