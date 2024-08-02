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
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Point<double> roost;
    int n;
    cin >> roost.x >> roost.y >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<double> dp(1 << n, DBL_MAX);
    dp[0] = 0;
    for (int mask = 1; mask < 1 << n; mask++)
        for (int i = 0; i < n; i++) {
            if (!(mask & 1 << i)) continue;
            for (int j = 0; j <= i; j++) {
                if (!(mask & 1 << j)) continue;

                int submask = 1 << i | 1 << j;
                auto d = dist(points[i], points[j]);
                d += (submask == mask) ? min(dist(points[i], roost), dist(points[j], roost)) : dist(points[i], roost) + dist(points[j], roost);

                dp[mask] = min(dp[mask], dp[mask ^ submask] + d);
            }
        }

    cout << fixed << setprecision(6) << dp.back();
}
