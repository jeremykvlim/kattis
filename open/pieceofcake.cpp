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
    double lg_nck = 0, lg_kci = 0;
    for (int i = 0; i < k; i++) lg_nck += log10(n - i) - log10(i + 1);
    for (int i = 0; i < n - k + 1; i++) {
        for (int j = 0; j < n; j++) ways[j][(n - k + 1 - i + j) % n] = lg_kci;
        lg_kci += log10(k + i - 1) - log10(i + 1);
    }

    double area = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (ways[i][j] != -1) area += pow(10, ways[i][j] - lg_nck) * cross(points[i], points[j]);

    cout << fixed << setprecision(6) << abs(area) / 2;
}
