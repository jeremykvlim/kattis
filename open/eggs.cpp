#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
double dist(Point<T> a, Point<T> b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, v;
    while (cin >> n >> v && n && v) {
        vector<Point<int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;
        points.emplace_back(0, 0);

        vector<double> sun(n);
        vector<vector<double>> time(n, vector<double>(n + 1));
        for (int i = 0; i < n; i++) {
            sun[i] = (720 + (double) points[i].x / 2000) * 60;

            for (int j = 0; j <= n; j++) time[i][j] = dist(points[i], points[j]) / v;
        }

        vector<double> dp(1 << n, INT_MAX);
        dp[0] = 0;
        int eggs = 0;
        for (int mask = 0; mask < 1 << n; mask++) {
            for (int i = 0; i < n; i++) {
                if (mask & 1 << i) continue;

                auto ti = time[i][n];
                if (dp[mask] + 2 * ti < sun[i]) dp[mask | 1 << i] = min(dp[mask | 1 << i], dp[mask] + 3 * ti);
                for (int j = 0; j < i; j++) {
                    if (mask & 1 << j) continue;

                    int submask = 1 << i | 1 << j;
                    auto tj = time[j][n];
                    if (dp[mask] + 4 * ti < sun[i] && dp[mask] + 4 * ti + time[i][j] * 2 < sun[j])
                        dp[mask | submask] = min(dp[mask | submask], dp[mask] + (4 * ti + tj + time[i][j] * 2));

                    if (dp[mask] + 4 * tj < sun[j] && dp[mask] + 4 * tj + time[j][i] * 2 < sun[i])
                        dp[mask | submask] = min(dp[mask | submask], dp[mask] + (4 * tj + ti + time[j][i] * 2));
                }
            }

            if (dp[mask] != INT_MAX) eggs = max(eggs, __builtin_popcount(mask));
        }

        cout << eggs << "\n";
    }
}
