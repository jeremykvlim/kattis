#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    Point operator-(Point p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double dist(Point<T> a, Point<T> b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
struct Line {
    Point<T> a, b;
    int i, j;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
    Line(Point<T> a, Point<T> b, int i, int j) : a(a), b(b), i(i), j(j) {}
    Line(T &a, T &b, T &c) {
        if (fabs(a) < 1e-8 && fabs(b) > 1e-8) {
            a = {0, c / b};
            b = {1, c / b};
        } else if (fabs(b) < 1e-8) {
            a = {c / a, 0};
            b = {c / a, 1};
        } else if (fabs(c) < 1e-8) {
            a = {0, c / b};
            b = {1, (c - a) / b};
        } else {
            a = {0, c / b};
            b = {c / a, 0};
        }
    }
};

template <typename T>
T area_of_parallelogram(const Line<T> l1, const Line<T> l2) {
    return cross(l1.b - l1.a, l2.b - l2.a);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    vector<Point<long long>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;
    sort(points.begin(), points.end());

    vector<Line<long long>> lines;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) lines.emplace_back(points[i], points[j], i, j);
    sort(lines.begin(), lines.end(), [&](auto l1, auto l2) {return area_of_parallelogram(l1, l2) < 0;});

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    int count = 2;
    for (auto li : lines) {
        swap(points[indices[li.i]], points[indices[li.j]]);
        swap(indices[li.i], indices[li.j]);

        int mid1 = indices[li.i], mid2 = indices[li.j];
        if (mid1 > mid2) swap(mid1, mid2);

        long long d = cross(points[mid1], li.a - li.b), limit = dist(li.a, li.b) * t;
        auto search = [&](int l, int r, bool half) {
            if (l + 1 >= r) return;

            while (l + 1 < r) {
                int mid = l + (r - l) / 2;

                if (abs(cross(li.a - li.b, points[mid]) + d) <= abs(limit)) (half ? r : l) = mid;
                else (half ? l : r) = mid;
            }

            count = max(count, (half ? mid2 - (~l ? l : r - 1) : l + 1 - mid1));
        };
        search(-1, mid1, true);
        search(mid2 - 1, n, false);
    }
    cout << count;
}
