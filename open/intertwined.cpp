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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull {
    deque<pair<Point<T>, int>> dq;

    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (dq.size() > 1 && violates(dq[1].first, dq[0].first, p.first)) dq.pop_front();
        dq.emplace_front(p);
    }

    int size() const {
        return dq.size();
    }

    auto & operator[](int i) {
        return dq[i];
    }

    const auto & operator[](int i) const {
        return dq[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double d;
    cin >> n >> d;

    bool collide = false;
    vector<Point<long long>> coords(n + 1, {0, 0});
    for (int i = 1; i <= n; i++) {
        cin >> coords[i].x >> coords[i].y;

        if (euclidean_dist(coords[i]) <= d) collide = true;
    }

    if (!collide) {
        cout << -1;
        exit(0);
    }

    vector<vector<Point<long long>>> rotations(4, vector<Point<long long>>(n + 1));
    for (int q = 0; q < 4; q++)
        for (int j = 0; j <= n; j++) {
            auto p = coords[j];
            if (!q) rotations[q][j] = p;
            else if (q == 1) rotations[q][j] = -~p;
            else if (q == 2) rotations[q][j] = -p;
            else rotations[q][j] = ~p;
        }

    vector<vector<int>> quadrant_order(4, vector<int>(n));
    for (int q = 0; q < 4; q++) {
        iota(quadrant_order[q].begin(), quadrant_order[q].end(), 1);
        sort(quadrant_order[q].begin(), quadrant_order[q].end(), [&](int i, int j) { return rotations[q][i] < rotations[q][j]; });
    }

    vector<int> order{0};
    for (int i = 0, count = 0, q = 0;; ++q %= 4) {
        vector<pair<Point<long long>, int>> points;
        for (int j = 1; j <= n; j++) points.emplace_back(rotations[q][j] - rotations[q][i], j);

        vector<pair<Point<long long>, int>> temp;
        for (int j : quadrant_order[q]) {
            auto diff = points[j - 1].first;
            if (diff.x < 0 || diff.y < 0) continue;
            temp.emplace_back(diff, j - 1);
        }

        if (temp.empty()) {
            next:;
            if (++count == 4) {
                cout << i;
                exit(0);
            }
            continue;
        }

        sort(temp.begin(), temp.end(), [&](auto p1, auto p2) { return p1.first < p2.first; });
        if (temp.front().first != Point<long long>{0, 0}) temp.insert(temp.begin(), {Point<long long>{0, 0}, 0});

        MonotonicHull<long long> hull;
        vector<vector<int>> adj_list(temp.size());
        for (int j = 0; j < temp.size(); j++) {
            hull.add(pair{temp[j].first, j});
            if (hull.size() > 1) adj_list[hull[1].second].emplace_back(j);
        }

        vector<int> indices;
        auto len = d;
        int v = 0;
        while (len >= 0) {
            auto it = find_if(adj_list[v].rbegin(), adj_list[v].rend(), [&](int u) { return len >= euclidean_dist(temp[v].first, temp[u].first); });
            if (it == adj_list[v].rend()) break;

            int u = *it;
            len -= euclidean_dist(temp[v].first, temp[u].first);
            v = u;
            indices.emplace_back(temp[u].second);
        }
        if (indices.empty()) goto next;

        count = 0;
        d = len;
        i = points[indices.back()].second;

        for (int j : indices) order.emplace_back(points[j].second);
        if (order.size() > 2)
            for (int k = order.size() - 2, l = order.size() - 1; k; k--)
                if (order[k - 1] == order[l - 1] && order[k] == order[l]) {
                    len = 0;
                    for (int j = k; j < l; j++) len += euclidean_dist(coords[order[j + 1]], coords[order[j]]);

                    if (d > len) d -= (int) (d / len - 1) * len;
                    break;
                }
    }
}
