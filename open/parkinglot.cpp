#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(U x, U y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    const auto begin() const {
        return &x;
    }

    const auto end() const {
        return &y + 1;
    }

    Point operator-() const {
        return {-x, -y};
    }

    bool operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    bool operator<=(const Point &p) const {
        return *this < p || *this == p;
    }

    bool operator>=(const Point &p) const {
        return *this > p || *this == p;
    }

    Point operator+(const Point &p) const {
        return {x + p.x, y + p.y};
    }

    Point operator+(const T &v) const {
        return {x + v, y + v};
    }

    Point & operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point & operator+=(const T &v) {
        x += v;
        y += v;
        return *this;
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }

    Point operator-(const T &v) const {
        return {x - v, y - v};
    }

    Point & operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point & operator-=(const T &v) {
        x -= v;
        y -= v;
        return *this;
    }

    Point operator*(const T &v) const {
        return {x * v, y * v};
    }

    Point & operator*=(const T &v) {
        x *= v;
        y *= v;
        return *this;
    }

    Point operator/(const T &v) const {
        return {x / v, y / v};
    }

    Point & operator/=(const T &v) {
        x /= v;
        y /= v;
        return *this;
    }
};

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<string> lot(r);
    for (auto &row : lot) cin >> row;

    vector<vector<bool>> entry(r + 1, vector<bool>(c + 1, false)), exit(r + 1, vector<bool>(c + 1, false));
    for (int i = 0; i <= r; i++)
        for (int j = 0; j <= c; j++) {
            entry[i][j] = j && i < r && lot[i][j - 1] == '#';
            exit[i][j] = i && j < c && lot[i - 1][j] == '#';
        }

    vector<vector<double>> dp(r + 1, vector<double>(c + 1, 1e9));
    dp[0][0] = 0;
    for (int R = 0; R <= r; R++)
        for (int C = 0; C <= c; C++) {
            vector<Point<int>> points;
            for (int i = 0; i <= R; i++)
                for (int j = 0; j <= C; j++)
                    if (i != R || j != C) points.emplace_back(i, j);

            Point<int> p{R, C};
            sort(points.begin(), points.end(), [&](auto p1, auto p2) { return cross(p1, p2, p) > 0; });

            multiset<int, greater<>> blocks;
            for (auto q : points) {
                auto [i, j] = q;
                if (entry[i][j] && i != R) blocks.insert(i + j);
                if (exit[i][j] && j != C) blocks.erase(blocks.lower_bound(i + j));
                if (i + j >= *blocks.begin()) dp[R][C] = min(dp[R][C], dp[i][j] + euclidean_dist(p, q));
            }
        }
    cout << setprecision(6) << fixed << dp[r][c];
}
