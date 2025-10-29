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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<Point<int>> points(n);
        vector<int> X(n), Y(n);
        for (int i = 0; i < n; i++) {
            cin >> points[i].x >> points[i].y;

            X[i] = points[i].x;
            Y[i] = points[i].y;
        }
        sort(X.begin(), X.end());
        sort(Y.begin(), Y.end());
        X.erase(unique(X.begin(), X.end()), X.end());
        Y.erase(unique(Y.begin(), Y.end()), Y.end());

        int x_size = X.size(), y_size = Y.size();
        vector<vector<int>> adj_list_x(x_size), adj_list_y(y_size);
        for (auto [x, y] : points) {
            int xi = lower_bound(X.begin(), X.end(), x) - X.begin(), yi = lower_bound(Y.begin(), Y.end(), y) - Y.begin();
            adj_list_x[xi].emplace_back(y);
            adj_list_y[yi].emplace_back(x);
        }

        auto check = [&](auto &adj_list) {
            for (auto &neighbors : adj_list) {
                if (neighbors.size() & 1) return false;
                sort(neighbors.begin(), neighbors.end());
            }
            return true;
        };
        if (!check(adj_list_x) || !check(adj_list_y)) {
            cout << "-1\n";
            continue;
        }

        auto sorted = points;
        sort(sorted.begin(), sorted.end());
        auto [x, y] = points[n - 1];
        auto len = 0LL;
        vector<bool> visited(n, false);
        for (bool dir = false;; dir = !dir) {
            int v = lower_bound(sorted.begin(), sorted.end(), Point(x, y)) - sorted.begin();
            if (v >= n || visited[v]) break;

            visited[v] = true;
            if (!dir) {
                int xi = lower_bound(X.begin(), X.end(), x) - X.begin(), yi = lower_bound(adj_list_x[xi].begin(), adj_list_x[xi].end(), y) - adj_list_x[xi].begin();
                yi ^= 1;
                if (yi >= adj_list_x[xi].size()) break;

                len += abs(y - adj_list_x[xi][yi]);
                y = adj_list_x[xi][yi];
            } else {
                int yi = lower_bound(Y.begin(), Y.end(), y) - Y.begin(), xi = lower_bound(adj_list_y[yi].begin(), adj_list_y[yi].end(), x) - adj_list_y[yi].begin();
                xi ^= 1;
                if (xi >= adj_list_y[yi].size()) break;

                len += abs(x - adj_list_y[yi][xi]);
                x = adj_list_y[yi][xi];
            }
        }

        if (all_of(visited.begin(), visited.end(), [&](bool b) { return b; })) cout << len << "\n";
        else cout << "-1\n";
    }
}
