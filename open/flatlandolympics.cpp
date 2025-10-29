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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Point<long long> s, e;
    int n;
    cin >> s.x >> s.y >> e.x >> e.y >> n;

    vector<Point<long long>> up, down;
    int left = 0, right = 0;
    for (int i = 0; i < n; i++) {
        Point<long long> p;
        cin >> p.x >> p.y;

        if (cross(e - s, p - s) > 0) up.emplace_back(p);
        else if (cross(e - s, p - s) < 0) down.emplace_back(p);
        else if (dot(e - s, p - s) < 0) left++;
        else right++;
    }

    long long count = left * (left - 1) / 2 + right * (right - 1) / 2;

    auto inversions = [&](auto points) {
        int n = points.size();

        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);

        vector<int> pref_s(n);
        sort(indices.begin(), indices.end(), [&](int i, int j) { return cross(points[i] - s, points[j] - s) > 0; });
        for (int i = 1; i < n; i++) pref_s[indices[i]] = pref_s[indices[i - 1]] + (cross(points[indices[i - 1]] - s, points[indices[i]] - s) > 0);

        vector<int> pref_e(n);
        sort(indices.begin(), indices.end(), [&](int i, int j) { return cross(points[i] - e, points[j] - e) < 0; });
        for (int i = 1; i < n; i++) pref_e[indices[i]] = pref_e[indices[i - 1]] + (cross(points[indices[i - 1]] - e, points[indices[i]] - e) < 0);

        sort(indices.begin(), indices.end(), [&](int i, int j) { return pref_s[i] < pref_s[j] || (pref_s[i] == pref_s[j] && pref_e[i] < pref_e[j]); });
        FenwickTree<int> fw(n + 1);
        auto inversions = 0LL;
        for (int i = 0; i < n; i++) {
            inversions += fw.pref_sum(pref_e[indices[i]] + 1);
            fw.update(pref_e[indices[i]] + 1, 1);
        }
        return inversions;
    };

    count += inversions(up);
    swap(s, e);
    count += inversions(down);

    cout << count;
}
