#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e6 + 3;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point<T> &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
};

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;
    sort(points.begin(), points.end());

    vector<vector<int>> count(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = i + 1; k < j; k++)
                if (cross(points[i], points[j], points[k]) > 0) {
                    count[i][j] = ++count[j][i];
                }

    auto sum = 0LL;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++) {
                auto cross_product = cross(points[i], points[j], points[k]);
                int c = count[i][j] + count[j][k] - count[k][i];
                if (cross_product < 0) {
                    cross_product *= -1;
                    c = -c - 1;
                }

                sum += (long long) cross_product * (c * 3 + n - 3 - c) % MODULO;
            }
    sum %= MODULO;
    if (sum & 1) sum += MODULO;

    cout << sum / 2;
}
