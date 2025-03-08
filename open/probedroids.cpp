#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-5;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    Point operator-() const {
        return {-x, -y};
    }

    bool operator<(const Point &p) const {
        return !approximately_equal(x, p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y);
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

    struct PointHash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

long long destroyed(long long n, long long m, long long mid) {
    if (mid < n) return (__gcd(m - 1, mid - 1) + m * mid - 1) / 2;
    else if (mid < n + m) return (__gcd(n - 1, n + m - mid - 1) + mid * n - (n - m) * n - 1) / 2;
    else return n * m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    int q;
    cin >> n >> m >> q;

    vector<Point<long long>> drdc(max(n, m));
    while (q--) {
        long long i;
        cin >> i;

        if (i < m) cout << "1 " << i + 1 << "\n";
        else if (i > n * (m - 1)) cout << i - n * (m - 1) + 1 << " 1\n";
        else {
            long long l = 0, r = INT_MAX, mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;
                
                if (destroyed(n, m, mid) >= i) r = mid;
                else l = mid;
            }
            i -= destroyed(n, m, l);

            Point<long long> curr = {0, 1}, remaining = {l - 1, m - 1};
            if (n <= l) remaining += n - l;

            auto valid = [&](Point<long long> p) {
                return p.x < n && p.y < m;
            };

            for (Point<long long> next = {curr.x + 1, curr.y}; valid(next) && cross(next, remaining) <= 0; next = {++curr.x + 1, curr.y});
            drdc[0] = {1, 0};
            for (int k = 1;;) {
                auto next = curr + drdc[k - 1];

                if (valid(next)) (cross(next, remaining) > 0 ? drdc[k++] : curr) = next;
                else {
                    if (cross(curr, remaining) > 0) {
                        auto j = min((n - 1) / curr.x, (m - 1) / curr.y);
                        
                        if (i <= j) {
                            cout << curr.x * i + 1 << " " << curr.y * i + 1 << "\n";
                            break;
                        }

                        i -= j;
                    }

                    curr = drdc[--k];
                }
            }
        }
    }
}
