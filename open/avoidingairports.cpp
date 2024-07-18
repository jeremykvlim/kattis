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

    auto operator*(Point<T> p) const {
        return x * p.y - p.x * y;
    }
};

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

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges(m);
    vector<tuple<long long, int, bool>> times(2 * m);
    for (int i = 0; i < m; i++) {
        int a, b, s, e;
        cin >> a >> b >> s >> e;

        edges[i] = {a, b};
        times[2 * i] = {s, i, false};
        times[2 * i + 1] = {e, i, true};
    }
    sort(times.begin(), times.end(), [](auto t1, auto t2) {return get<0>(t1) != get<0>(t2) ? get<0>(t1) < get<0>(t2) : get<2>(t1) < get<2>(t2);});

    vector<deque<Point<long long>>> trick_hulls(n + 1);
    vector<long long> dp(m, LLONG_MAX);
    trick_hulls[1].emplace_back(0, 0);
    auto sum = LLONG_MAX;
    for (auto [t, i, end] : times) {
        auto [v, u] = edges[i];
        if (end) {
            if (dp[i] == LLONG_MAX) continue;

            add(trick_hulls[u], {2 * t, dp[i] + t * t});
        } else {
            auto f = [&](auto p) {
                return -p.x * t + p.y;
            };
            while (trick_hulls[v].size() > 1 && f(trick_hulls[v].back()) >= f(trick_hulls[v][trick_hulls[v].size() - 2])) trick_hulls[v].pop_back();

            if (!trick_hulls[v].empty()) dp[i] = min(dp[i], t * t + f(trick_hulls[v].back()));
        }

        if (u == n) sum = min(sum, dp[i]);
    }

    cout << sum;
}
