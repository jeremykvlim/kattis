#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<vector<double>> ways(n, vector<double>(n, -1));
    for (int i = 0; i <= n - k; i++) {
        auto lg_kci = lgamma(k + i - 1) - lgamma(k - 1) - lgamma(i + 1);
        for (int j = 0; j < n; j++) ways[j][(j - k + 1 - i + n) % n] = lg_kci;
    }

    double area = 0, lg_nck = lgamma(n + 1) - lgamma(k + 1) - lgamma(n - k + 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (ways[i][j] != -1) area += exp(ways[i][j] - lg_nck) * cross(points[i], points[j]);

    cout << fixed << setprecision(6) << abs(area) / 2;
}
