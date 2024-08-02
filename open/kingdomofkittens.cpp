#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(Point &p) const {
        return x == p.x && y == p.y;
    }

    auto operator!=(Point &p) const {
        return x != p.x || y != p.y;
    }

    Point operator-(Point &p) const {
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
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

template <typename T>
bool collinear(Point<T> c, Point<T> a, Point<T> b) {
    return cross(a - b, a - c) == 0;
}

template <typename T>
bool between(Point<T> c, Point<T> a, Point<T> b) {
    return collinear(c, a, b) && dot(a - c, b - c) < 0;
}

template <typename T>
deque<Point<T>> monotone(vector<Point<T>> &points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return deque<Point<T>>(points.begin(), points.end());

    deque<Point<T>> convex_hull;
    for (auto p : points) {
        while (convex_hull.size() > 1 && cross(convex_hull[1], convex_hull[0], p) <= 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && cross(convex_hull[1], convex_hull[0], p) <= 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    convex_hull.pop_front();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<long long>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;

        auto convex_hull = monotone(points);
        int size = convex_hull.size();

        if (size <= 2) {
            cout << "YES\n";
            continue;
        }

        auto next = [&](int i) {
            return (i + 1) % size;
        };

        auto forms_hull = [&]() {
            auto vertex_or_on_side = [&](Point<long long> c) {
                for (int i = 0; i < size; i++) {
                    auto a = convex_hull[i], b = convex_hull[next(i)];
                    if (a == c || b == c || between(c, a, b)) return true;
                }

                return false;
            };

            return all_of(points.begin(), points.end(), [&](auto c) {return vertex_or_on_side(c);});
        };

        if (size > 6 || !forms_hull()) {
            cout << "NO\n";
            continue;
        }

        vector<bool> on_side(size, false);
        for (int i = 0; i < size; i++) {
            auto a = convex_hull[i], b = convex_hull[next(i)];
            for (auto c : points)
                if (a != c && b != c && between(c, a, b)) on_side[i] = true;
        }

        auto valid = [&](int i, int j) -> bool {
            auto cross_product = cross(convex_hull[next(i)] - convex_hull[i], convex_hull[next(j)] - convex_hull[j]);
            if (cross_product == 0) return false;
            if (cross_product < 0) swap(i, j);

            for (int k = next(i); k != j; k = next(k))
                if (on_side[k] || next(k) != j) return false;

            int count = 0;
            for (int k = next(j); k != i; k = next(k)) count++;
            if (count <= 1) return true;
            if (count >= 4) return false;
            
            if (count == 2) {
                if (on_side[next(j)] && (on_side[next(j + 1)] || cross(convex_hull[next(j + 1)] - convex_hull[next(j)], convex_hull[next(i)] - convex_hull[i]) <= 0) ||
                    on_side[next(j + 1)] && cross(convex_hull[next(j)] - convex_hull[j], convex_hull[next(j + 2)] - convex_hull[next(j + 1)]) <= 0) return false;

                return true;
            }
            
            if (count == 3) {
                if (on_side[next(j)] || on_side[next(j + 2)]) return false;
                if (cross(convex_hull[next(j)] - convex_hull[j], convex_hull[next(j + 2)] - convex_hull[next(j + 1)]) > 0 &&
                    cross(convex_hull[next(j + 2)] - convex_hull[next(j + 1)], convex_hull[next(i)] - convex_hull[i]) > 0) return true;

                return false;
            }
        };

        for (int i = 0; i < size; i++)
            for (int j = i + 1; j < size; j++)
                if (valid(i, j)) {
                    cout << "YES\n";
                    goto next;
                }

        cout << "NO\n";
        next:;
    }
}
