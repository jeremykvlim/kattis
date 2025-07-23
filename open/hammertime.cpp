#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto radius = 2.54 / 2 + 0.25;

    int n, m;
    while (cin >> n >> m && n && m) {
        vector<vector<double>> height(n, vector<double>(n, 5));
        while (m--) {
            double X, Y;
            cin >> X >> Y;

            vector<tuple<double, int, int>> nails;
            for (int r = 0; r < n; r++) {
                auto dy = Y - r - 1;
                for (int c = 0; c < n; c++) {
                    auto dx = X - c - 1;
                    if (dx * dx + dy * dy <= radius * radius) nails.emplace_back(height[r][c], r, c);
                }
            }
            sort(nails.rbegin(), nails.rend());

            int k = nails.size();
            double remaining = 1;
            vector<double> dist(k, 0);
            for (int i = 1; i <= k && remaining > 1e-9; i++) {
                auto delta = max(0., get<0>(nails[i - 1]) - get<0>(nails[i]));
                if (i == k || remaining < i * delta) {
                    for_each(dist.begin(), dist.begin() + i, [&](auto &d) { d += remaining / i; });
                    break;
                }
                for_each(dist.begin(), dist.begin() + i, [&](auto &d) { d += delta; });
                remaining -= i * delta;
            }

            for (int i = 0; i < k; i++) {
                auto [h, r, c] = nails[i];
                height[r][c] = max(0., height[r][c] - dist[i]);
            }
        }

        for (auto row : height) {
            for (auto h : row) cout << fixed << setprecision(3) << h << " ";
            cout << "\n";
        }
        cout << "\n";
    }
}
