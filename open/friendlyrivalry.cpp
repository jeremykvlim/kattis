#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
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
};

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int c = 2 * n;
    vector<Point<long long>> coords(c);
    for (auto &[x, y] : coords) cin >> x >> y;

    vector<double> dist;
    for (int i = 0; i < c; i++)
        for (int j = i + 1; j < c; j++) dist.emplace_back(euclidean_dist(coords[i], coords[j]));
    sort(dist.begin(), dist.end());
    dist.erase(unique(dist.begin(), dist.end(), [&](double d1, double d2) { return abs(d1 - d2) < 1e-9; }), dist.end());

    double distance = 0;
    vector<int> blue;
    int l = -1, r = dist.size(), mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        DisjointSets dsu(c);
        for (int i = 0; i < c; i++)
            for (int j = i + 1; j < c; j++)
                if (euclidean_dist(coords[i], coords[j]) + 1e-9 < dist[mid]) dsu.unite(i, j);

        vector<vector<int>> components(c);
        for (int i = 0; i < c; i++) components[dsu.find(i)].emplace_back(i);

        vector<int> prev(n + 1);
        vector<bool> visited1(n + 1, false);
        visited1[0] = true;
        for (int i = 0; i < c; i++)
            for (int j = n, s = components[i].size(); j >= s; j--)
                if (visited1[j - s] && !visited1[j]) {
                    prev[j] = i;
                    visited1[j] = true;
                }

        if (!visited1[n]) r = mid;
        else {
            distance = dist[mid];

            vector<bool> visited2(c, false);
            int temp = n;
            while (temp) {
                visited2[prev[temp]] = true;
                temp -= components[prev[temp]].size();
            }

            vector<int> b;
            for (int i = 0; i < c; i++)
                if (visited2[i])
                    for (int chapter : components[i]) b.emplace_back(chapter);

            blue = b;
            l = mid;
        }
    }

    cout << fixed << setprecision(6) << distance << "\n";
    for (int chapter : blue) cout << chapter + 1 << "\n";
}
