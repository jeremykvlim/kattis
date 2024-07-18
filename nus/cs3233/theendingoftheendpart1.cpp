#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
    
    Point operator-(Point<T> &p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
double cross(Point<T> a, Point<T> b) {
    return (double) (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

template <typename T>
void add(deque<Point<T>> &trick_hull, Point<T> p) {
    while (trick_hull.size() > 1 && cross(trick_hull[1], trick_hull[0], p) > 0) trick_hull.pop_front();
    trick_hull.emplace_front(p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<Point<long long>> e(r), n(c);
    auto read = [](vector<Point<long long>> &d, int size) {
        for (int i = 0; i < size; i++) {
            long long s;
            cin >> s;

            d[i] = {i, s};
        }
    };
    read(e, r);
    read(n, c);

    deque<Point<long long>> east, north;
    for (auto p : e) add(east, p);
    for (auto p : n) add(north, p);

    vector<pair<Point<long long>, char>> route;
    for (int i = east.size() - 1; i; i--) route.emplace_back(east[i - 1] - east[i], 'N');
    for (int i = north.size() - 1; i; i--) route.emplace_back(north[i - 1] - north[i], 'E');
    sort(route.begin(), route.end(), [&](auto p1, auto p2) {return cross(p1.first, p2.first) > 0;});

    for (auto [point, direction] : route)
        while (point.x--) cout << direction;
}
