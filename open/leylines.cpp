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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
auto euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    using U = common_type_t<T, double>;
    U x = (U) a.x - b.x, y = (U) a.y - b.y;
    return sqrt(x * x + y * y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    vector<Point<long long>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;
    sort(points.begin(), points.end());

    vector<tuple<Point<long long>, int, int>> d(n * (n - 1) / 2);
    for (int i = 0, k = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) d[k++] = {points[i] - points[j], i, j};
    sort(d.begin(), d.end(), [&](const auto &t1, const auto &t2) { return cross(get<0>(t1), get<0>(t2)) < 0; });

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    int count = 2;
    for (const auto &[v, i, j] : d) {
        swap(points[indices[i]], points[indices[j]]);
        swap(indices[i], indices[j]);

        int mid = min(indices[i], indices[j]), left = mid - count + 1, right = mid + count;
        if (left < 0 && right >= n) continue;

        long long limit = euclidean_dist(v) * t;
        if (left >= 0 && abs(cross(points[left] - points[mid], v)) <= limit) {
            int l = -1, r = left, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (abs(cross(points[m] - points[mid], v)) <= limit) r = m;
                else l = m;
            }
            count = max(count, mid - r + 2);
        }

        if (right < n && abs(cross(points[right] - points[mid], v)) <= limit) {
            int l = right, r = n, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (abs(cross(points[m] - points[mid], v)) <= limit) l = m;
                else r = m;
            }
            count = max(count, l - mid + 1);
        }

        if (count == n) break;
    }
    cout << count;
}
