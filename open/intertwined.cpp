#include <bits/stdc++.h>
using namespace std;

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

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
void add(deque<pair<Point<T>, int>> &half_hull, vector<vector<int>> &adj_list, pair<Point<T>, int> p, bool collinear = false) {
    auto clockwise = [&]() {
        T cross_product = cross(half_hull[1].first, half_hull[0].first, p.first);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    while (half_hull.size() > 1 && clockwise()) half_hull.pop_front();
    adj_list[half_hull[0].second].emplace_back(p.second);
    half_hull.emplace_front(p);
}

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

    vector<int> order{0};
    for (int i = 0, count = 0, quadrant = 0;; ++quadrant %= 4) {
        vector<pair<Point<long long>, int>> points;
        for (int j = 1; j <= n; j++) points.emplace_back(coords[j] - coords[i], j);

        auto rotate = [&](auto &p) {
            if (quadrant == 1) p = {p.y, -p.x};
            else if (quadrant == 2) p = {-p.x, -p.y};
            else p = {-p.y, p.x};
        };
        if (quadrant)
            for (auto &[p, _] : points) rotate(p);

        auto temp = points;
        for (int j = 0; j < temp.size(); j++) temp[j].second = j;

        temp.erase(remove_if(temp.begin(), temp.end(), [&](auto p) {return p.first.x < 0 || p.first.y < 0;}), temp.end());
        if (temp.empty()) {
            next:;
            if (++count == 4) {
                cout << i;
                exit(0);
            }

            continue;
        }

        vector<int> indices;
        if (!temp.empty()) {
            sort(temp.begin(), temp.end(), [&](auto p1, auto p2) {return p1.first < p2.first;});
            if (temp.front().first != Point<long long>{0, 0}) temp.insert(temp.begin(), {Point<long long>{0, 0}, 0});

            deque<pair<Point<long long>, int>> half_hull{{Point<long long>{0, 0}, 0}};
            vector<vector<int>> adj_list(temp.size());
            for (int j = 1; j < temp.size(); j++) add(half_hull, adj_list, {temp[j].first, j});

            int v = 0;
            for (auto len = d; len >= 0;) {
                auto it = find_if(adj_list[v].rbegin(), adj_list[v].rend(), [&](int u) {return len >= euclidean_dist(temp[v].first, temp[u].first);});
                if (it == adj_list[v].rend()) break;

                int u = *it;
                len -= euclidean_dist(temp[v].first, temp[u].first);
                v = u;
                indices.emplace_back(temp[u].second);
            }
            if (indices.empty()) goto next;
        }

        count = 0;
        d -= euclidean_dist(points[indices[0]].first);
        for (int j = 1; j < indices.size(); j++) d -= euclidean_dist(points[indices[j]].first, points[indices[j - 1]].first);
        i = points[indices.back()].second;

        for (int j : indices) order.emplace_back(points[j].second);
        if (order.size() >= 3)
            for (int k = order.size() - 2, l = order.size() - 1; ~k; k--)
                if (order[k - 1] == order[l - 1] && order[k] == order[l]) {
                    double len = 0;
                    for (int j = k; j < l; j++) len += euclidean_dist(coords[order[j + 1]], coords[order[j]]);

                    if (d > len) d -= (int) (d / len - 1) * len;
                    break;
                }
    }
}
