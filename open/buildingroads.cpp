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

    int n;
    cin >> n;

    vector<Point<int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    double len = 0;
    vector<vector<double>> d(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++) {
            d[i][j] = d[j][i] = dist(points[i], points[j]);
            len += d[i][j];
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) d[j][k] = min(d[j][k], d[j][i] + d[i][k]);

    vector<vector<int>> indices(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        iota(indices[i].begin(), indices[i].end(), 0);
        sort(indices[i].begin(), indices[i].end(), [&](int j, int k) {return d[i][j] < d[i][k];});
    }
    for (int i = 0; i < n; i++) len = min(len, d[i][indices[i][n - 1]] * 2);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            int longest = indices[i][n - 1];
            for (int k = n - 1; ~k; k--)
                if (d[j][indices[i][k]] > d[j][longest]) {
                    len = min(len, d[i][indices[i][k]] + d[i][j] + d[j][longest]);
                    longest = indices[i][k];
                }
        }

    cout << fixed << setprecision(3) << len;
}
