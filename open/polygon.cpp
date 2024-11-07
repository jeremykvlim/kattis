#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    auto operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<int>> points(2 * n + 1);
        for (int i = 0; i < n; i++) cin >> points[i].x >> points[i].y;
        points[n] = points[n - 1];
        for (int i = n; ~i; i--) {
            if (i) points[i + n] = points[i - 1];
            if (points[i + n] < points[i]) swap(points[i + n], points[i]);
        }

        vector<int> indices(2 * n + 1);
        iota(indices.begin(), indices.end(), 0);
        sort(indices.begin(), indices.end(), [&](int i, int j) {return points[i] < points[j] || points[i] == points[j] && i > j;});

        auto cmp = [&](int i, int j) {
            int cross_product = cross(points[j] - points[i], points[j] - points[j + n]);
            return cross_product < 0 || !cross_product && (cross(points[i] - points[i + n], points[j] - points[j + n])) < 0;
        };
        multiset<int, decltype(cmp)> ms(cmp);
        vector<decltype(ms)::const_iterator> its(n + 1);
        vector<bool> valid(n + 1, false);
        auto check = [&](int i, int j) {
            int c1 = cross(points[i] - points[i + n], points[i] - points[j + n]);
            if (c1 < 0) return false;

            int c2 = cross(points[j] - points[j + n], points[j] - points[i + n]);
            if (c2 > 0) return false;

            if (c1 || c2) return points[i] != points[j];
            else return !(cross(points[i] - points[i + n], points[j] - points[j + n]));
        };
        for (int i : indices)
            if (i < n) {
                auto it = its[i] = ms.emplace(i);
                valid[i] = true;
                if (it != ms.begin() && check(*prev(it), i) || ++it != ms.end() && check(i, *it)) {
                    cout << "NO\n";
                    goto next;
                }
            } else if (valid[i - n]) {
                auto it = ms.erase(its[i - n]);
                valid[i - n] = false;
                if (it != ms.begin() && it != ms.end() && check(*prev(it), *it)) {
                    cout << "NO\n";
                    goto next;
                }
            }
        cout << "YES\n";
        next:;
    }
}
