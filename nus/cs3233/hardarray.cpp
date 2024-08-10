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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<long long>> points(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x;

        if (i) points[i].x += points[i - 1].x;
    }

    for (int i = 0; i < n; i++) {
        cin >> points[i].y;

        if (i) points[i].y += points[i - 1].y;
    }
    sort(points.begin(), points.end());

    auto sq = [](long long v) -> __int128 {return (__int128) v * v;};

    __int128 d = 1e27;
    auto update = [&](auto p1, auto p2) {
        d = min(d, sq(p1.x - p2.x) + sq(p1.y - p2.y));
    };

    auto cmp = [](auto p1, auto p2) {return p1.y < p2.y;};
    multiset<Point<long long>, decltype(cmp)> ms(cmp);
    vector<decltype(ms)::const_iterator> its(n + 1);
    for (int i = 0, j = 0; i <= n; i++) {
        for (; j < i && sq(points[j].x - points[i].x) >= d; j++) ms.erase(its[j]);

        auto it = ms.upper_bound(points[i]);
        if (it != ms.begin()) {
            update(*prev(it), points[i]);
            for (auto p = prev(it); p != ms.begin() && sq(points[i].y - p->y) < d;) update(*--p, points[i]);
        }
        for (; it != ms.end() && sq(it->y - points[i].y) < d; it++) update(*it, points[i]);
        its[i] = ms.emplace_hint(ms.upper_bound(points[i]), points[i]);
    }

    cout << (long long) d;
}
