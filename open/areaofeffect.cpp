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

    Point operator!() const {
        return {y, x};
    }

    Point operator~() const {
        return {-y, x};
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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, R;
    cin >> n >> m >> R;

    vector<Circle<double>> villages(n);
    for (auto &c : villages) cin >> c.origin.x >> c.origin.y >> c.radius;

    vector<Point<double>> minions(m);
    for (auto &[x, y] : minions) cin >> x >> y;

    int destroyed = 1;
    for (int i = 0; i < m; i++) {
        vector<Point<double>> displacement(n);
        vector<double> p(n);
        for (int k = 0; k < n; k++) {
            displacement[k] = villages[k].origin - minions[i];
            p[k] = dot(displacement[k], displacement[k]) - villages[k].radius * villages[k].radius;
        }

        int count = 1;
        vector<pair<double, int>> sweep;
        for (int j = 0; j < m; j++)
            if (i != j) {
                auto dist = squared_dist(minions[i], minions[j]);
                if (dist > 4 * R * R) continue;

                auto v = minions[j] - minions[i];
                auto bounds = [&](auto v, auto limit) -> pair<Point<double>, Point<double>> {
                    auto d = dot(v, v);
                    auto perp = ~v * sqrt(max(0., d - limit * limit));
                    return {(v * limit - perp) / d, (v * limit + perp) / d};
                };
                auto [l, r] = bounds(v, dist / (2 * R));
                bool skip = false;
                for (int k = 0; k < n; k++) {
                    auto u = v * p[k] - displacement[k] * dist;
                    auto limit = dist * villages[k].radius;
                    
                    bool valid_l = dot(l, u) >= limit, valid_r = dot(r, u) >= limit;
                    if (valid_l && valid_r) continue;
                    else if (!valid_l && valid_r) l = bounds(u, limit).first;
                    else if (valid_l && !valid_r) r = bounds(u, limit).second;
                    else {
                        if (cross(l, u) < -1e-7 || cross(u, r) < -1e-7) {
                            skip = true;
                            break;
                        }
                        tie(l, r) = bounds(u, limit);
                    }
                }
                if (skip) continue;
                auto al = angle(l), ar = angle(r);
                if (al > ar) count++;
                sweep.emplace_back(al, 1);
                sweep.emplace_back(ar, -1);
            }
        sort(sweep.begin(), sweep.end());
        destroyed = max(destroyed, count);
        for (int l = 0, r = 0; l < sweep.size(); l = r) {
            int add = 0, remove = 0;
            for (; r < sweep.size() && sweep[l].first == sweep[r].first; r++)
                if (sweep[r].second > 0) add++;
                else remove++;

            count += add;
            destroyed = max(destroyed, count);
            count -= remove;
        }
    }
    cout << destroyed;
}
