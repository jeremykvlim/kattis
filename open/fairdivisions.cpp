#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n & 1) {
        cout << 0;
        exit(0);
    }

    vector<Point<long long>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    mt19937_64 rng(random_device{}());
    auto all1 = 0ULL, all2 = 0ULL;
    vector<unsigned long long> r1(n), r2(n);
    for (int i = 0; i < n; i++) {
        all1 ^= r1[i] = rng();
        all2 ^= r2[i] = rng();
    }

    unordered_set<pair<unsigned long long, unsigned long long>, Hash> splits;
    for (int i = 0; i < n; i++) {
        vector<tuple<Point<long long>, int, int>> sweep;
        int count = 1;
        auto h1 = 0ULL ^ r1[i], h2 = 0ULL ^ r2[i];
        vector<bool> blue(n, false);
        blue[i] = true;
        for (int j = 0; j < n; j++)
            if (i != j) {
                if (points[i].y < points[j].y || (points[i].y == points[j].y && points[i].x < points[j].x)) sweep.emplace_back(points[j] - points[i], j, 1);
                else {
                    sweep.emplace_back(points[i] - points[j], j, -1);
                    blue[j] = true;
                    count++;
                    h1 ^= r1[j];
                    h2 ^= r2[j];
                }
            }
        if (count == n / 2) splits.emplace(h1, h2);

        sort(sweep.begin(), sweep.end(), [&](const auto &t1, const auto &t2) {
            auto c = cross(get<0>(t1), get<0>(t2));
            return c ? c < 0 : get<0>(t1) < get<0>(t2);
        });

        for (int l = 0, r = 1; l < sweep.size(); l = r++) {
            for (; r < sweep.size() && !cross(get<0>(sweep[l]), get<0>(sweep[r])); r++);

            for (int j = l; j < r; j++) {
                auto [p, k, delta] = sweep[j];
                count += delta;
                blue[k] = !blue[k];
                h1 ^= r1[k];
                h2 ^= r2[k];
            }

            if (count == n / 2) {
                splits.emplace(h1, h2);
                splits.emplace(h1 ^ all1, h2 ^ all2);
            }
        }
    }
    cout << splits.size();
}
