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
T manhattan_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int main () {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, K, w;
    cin >> b >> K >> w;

    auto ask = [&](vector<Point<int>> probes) {
        cout << "? ";
        for (auto [x, y] : probes) cout << x << " " << y << " ";
        cout << "\n" << flush;

        vector<int> dist(K * probes.size());
        for (int &di : dist) cin >> di;
        return dist;
    };

    auto dist = ask({{-b, b}, {-b, -b}});
    vector<Point<int>> possible;
    for (int d1 : dist)
        for (int d2 : dist) {
            if ((d1 + d2) & 1) continue;

            int x = (d1 + d2 - 2 * b) / 2 - b, y = d1 - x - 2 * b;
            if (!(-b <= x && x <= b && -b <= y && y <= b)) continue;
            possible.emplace_back(x, y);
        }
    sort(possible.begin(), possible.end());
    possible.erase(unique(possible.begin(), possible.end()), possible.end());

    int d = possible.size();
    vector<Point<int>> probes(d);
    vector<vector<int>> indices(d);
    vector<pair<int, int>> candidates(d);
    vector<bool> selected(d, false), used(2 * d * d, false), visited(2 * d * d, false);
    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    for (int h = 0, d1 = 0; h < d; d1++)
        if (!visited[d1])
            for (int i = 0; i < d; i++) {
                if (selected[i]) continue;

                Point<int> p;
                for (int k = 0; k < 4; k++) {
                    int x = possible[i].x + dx[k] * d1, y = possible[i].y + dy[k] * d1;
                    if (!(-1e8 <= x && x <= 1e8 && -1e8 <= y && y <= 1e8)) continue;

                    p = {x, y};
                    for (int j = 0; j < d; j++) {
                        if (i == j) continue;
                        int d2 = manhattan_dist(p, possible[j]);
                        if (d2 >= 2 * d * d) continue;
                        if ((!selected[j] && d1 == d2) || used[d2]) goto next;
                    }
                    goto done;
                    next:;
                }
                continue;

                done:
                for (int j = 0; j < d; j++) {
                    if (i == j) continue;
                    int d2 = manhattan_dist(p, possible[j]);
                    if (d2 >= 2 * d * d) continue;
                    if (d1 == d2) indices[h].emplace_back(j);
                    visited[d2] = true;
                }
                probes[h] = p;
                candidates[h++] = {i, d1};
                selected[i] = used[d1] = true;
                break;
            }

    unordered_map<int, int> count;
    for (int di : ask(probes)) count[di]++;

    vector<Point<int>> locations(K);
    vector<bool> deposit(d, false);
    for (int i = 0, j = 0; i < d && j < K; i++) {
        int c = 1;
        for (int k : indices[i]) c += deposit[k];

        auto [k, di] = candidates[i];
        if (count[di] == c) {
            locations[j++] = possible[k];
            deposit[k] = true;
        }
    }
    cout << "! ";
    for (auto [x, y] : locations) cout << x << " " << y << " ";
    cout << flush;
}
