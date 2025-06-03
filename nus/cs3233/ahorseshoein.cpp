#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

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
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == -1 : sgn(y - p.y) == -1;
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == 1 : sgn(y - p.y) == 1;
        return x != p.x ? x > p.x : y > p.y;
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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    if (k <= 2) {
        for (int i = 1; i <= k; i++) cout << i << " ";
        exit(0);
    }

    k = n - k;
    vector<Point<long long>> polygon(2 * n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> polygon[i].x >> polygon[i].y;

        polygon[i + n] = polygon[i];
    }

    vector<long long> pref(2 * n + 1, 0);
    auto shoelace = [&](int i, int j) {
        return (polygon[i].x - polygon[j].x) * (polygon[i].y + polygon[j].y);
    };
    for (int i = 1; i < 2 * n; i++) pref[i + 1] = pref[i] + shoelace(i + 1, i);

    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    mt19937 rng(random_device{}());

    auto ratio = (double) k / n;
    vector<long long> dp((n + 1) * (k + 1), 0);
    vector<int> used((n + 1) * (k + 1), 0);
    auto index = [&](int i, int j) -> int {
        return i * (k + 1) + j;
    };
    auto largest = -1LL;
    vector<int> unplaced;
    for (double _ = 1; _ > 1e-6 && !perm.empty(); _ *= ratio) {
        int pos = rng() % perm.size(), offset = perm[pos];
        swap(perm[pos], perm.back());
        perm.pop_back();

        fill(dp.begin(), dp.end(), 0);
        fill(used.begin(), used.end(), 0);

        auto area = [&](int l, int r) -> long long {
            if (l < 1) {
                l += n;
                r += n;
            }
            return abs(pref[r] - pref[l] + shoelace(l, r));
        };

        for (int i = 2; i <= n; i++)
            for (int j = 0; j <= min(i, k); j++) {
                dp[index(i, j)] = dp[index(i - 1, j)];
                for (int l = 1; l <= j; l++) {
                    auto a = area(i + offset - l, i + offset + 1);
                    if (i > l && dp[index(i, j)] < dp[index(i - l - 1, j - l)] + a) {
                        dp[index(i, j)] = dp[index(i - l - 1, j - l)] + a;
                        used[index(i, j)] = l;
                    }
                }
            }

        vector<int> vertices;
        for (int i = n, j = k; ~i && ~j; i--) {
            int l = used[index(i, j)];
            if (l) {
                for (int m = 0; m < l; m++) {
                    int d = i + offset - m;
                    while (d > n) d -= n;
                    vertices.emplace_back(d);
                }
                i -= l;
                j -= l;
            }
        }

        if (largest < dp[index(n, k)]) {
            largest = dp[index(n, k)];
            unplaced = vertices;
        }
    }

    vector<bool> placed(n + 1, true);
    for (int d : unplaced) placed[d] = false;
    for (int d = 1; d <= n; d++)
        if (placed[d])  cout << d << " ";
}
