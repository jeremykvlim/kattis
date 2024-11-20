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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<double>> points(n);
        vector<pair<Point<double>, int>> sorted(n);
        for (int i = 0; i < n; i++) {
            cin >> points[i].x >> points[i].y;

            sorted[i] = {points[i], i};
        }
        sort(sorted.begin(), sorted.end(), [](auto p1, auto p2) {return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first;});

        auto sq = [](double v) -> double {return v * v;};

        auto d = DBL_MAX;
        int a = -1, b = -1;
        auto update = [&](auto p1, auto p2) {
            auto dist = sq(p1.first.x - p2.first.x) + sq(p1.first.y - p2.first.y);
            if (d > dist) {
                d = dist;
                a = p1.second;
                b = p2.second;
            }
        };

        auto cmp = [](auto p1, auto p2) {return p1.first.y < p2.first.y;};
        multiset<pair<Point<double>, int>, decltype(cmp)> ms(cmp);
        vector<decltype(ms)::const_iterator> its(n);
        for (int i = 0, j = 0; i < n; i++) {
            for (; j < i && sq(sorted[j].first.x - sorted[i].first.x) >= d; j++) ms.erase(its[j]);

            auto it = ms.upper_bound(sorted[i]);
            if (it != ms.begin()) {
                update(*prev(it), sorted[i]);
                for (auto p = prev(it); p != ms.begin() && sq(sorted[i].first.y - p->first.y) < d;) update(*--p, sorted[i]);
            }
            for (; it != ms.end() && sq(it->first.y - sorted[i].first.y) < d; it++) update(*it, sorted[i]);
            its[i] = ms.emplace_hint(ms.upper_bound(sorted[i]), sorted[i]);
        }

        cout << fixed << setprecision(2) << points[a].x << " " << points[a].y << " " << points[b].x << " " << points[b].y << "\n";
    }
}
