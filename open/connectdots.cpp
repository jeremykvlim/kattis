#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    auto operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    Point operator-() {
        return {-x, -y};
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
T dot(Point<T> a, Point<T> b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
bool collinear(Point<T> a, Point<T> b) {
    return cross(a, b) == 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Point<int>> indices(16);
    vector<vector<int>> grid(4, vector<int>(4));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            cin >> grid[i][j];

            indices[grid[i][j] - 1] = {i, j};
        }

    vector<int> dp(16, 16);
    dp[1] = 0;
    for (int i = 1; i < 15; i++) {
        auto p1 = indices[i] - indices[i - 1], p2 = indices[i + 1] - indices[i];

        dp[i + 1] = min(dp[i + 1], dp[i] + !(collinear(p1, p2) && dot(p1, p2) > 0));
        if (collinear(p1, p2)) continue;

        p2 = p1;
        for (int j = i + 1; j < 16; j++) {
            auto p3 = indices[j] - indices[j - 1];

            if (cross(p1, p3) > 0 && cross(p3, p2) > 0) {
                dp[j] = min(dp[j], dp[i] + j - i - 1);
                break;
            }

            p1 = -p1;
            p2 = -p2;

            if (cross(p1, p3) > 0 && cross(p3, p2) > 0 || collinear(p1, p3) && dot(p1, p3) > 0 || collinear(p3, p2) && dot(p3, p2) > 0) continue;

            if (cross(p3, p1) > 0) p1 = p3;
            else if (cross(p2, p3) > 0) p2 = p3;
        }
    }
    cout << dp[15] + 1;
}
