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
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

pair<int, unsigned __int128> bron_kerbosch(int n, vector<unsigned __int128> adj_list) {
    int max_clique_size = 0;
    unsigned __int128 clique_mask = 0;

    auto dfs = [&](auto &&self, unsigned __int128 p, unsigned __int128 r = 0) {
        if (!p) {
            int size = popcount(r);
            if (max_clique_size < size) {
                max_clique_size = size;
                clique_mask = r;
            }
            return;
        }

        if (popcount(r) + popcount(p) <= max_clique_size) return;

        auto lsb = [&](unsigned __int128 v) {
            return v ? ((v & ULLONG_MAX) ? countr_zero(v) : 64 + countr_zero(v >> 64)) : 0;
        };

        auto color_order = [&](unsigned __int128 p) -> vector<pair<int, int>> {
            int color = 0;
            vector<pair<int, int>> order;
            while (p) {
                color++;
                for (auto q = p; q;) {
                    int v = lsb(q);
                    auto mask = (unsigned __int128) 1 << v;
                    q &= q - 1;
                    order.emplace_back(v, color);
                    p &= ~mask;
                    q &= ~adj_list[v];
                }
            }
            reverse(order.begin(), order.end());
            return order;
        };

        for (auto [v, color] : color_order(p)) {
            if (popcount(r) + color <= max_clique_size) return;
            auto mask = (unsigned __int128) 1 << v;
            self(self, p & adj_list[v], r | mask);
            p &= ~mask;
        }
    };
    dfs(dfs, ((unsigned __int128) 1 << n) - 1);

    return {max_clique_size, clique_mask};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d;
    cin >> n >> d;

    vector<Point<int>> points(n);
    vector<unsigned __int128> adj_list(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;

        for (int j = i - 1; ~j; j--)
            if (squared_dist(points[i], points[j]) <= d * d) {
                adj_list[i] |= (unsigned __int128) 1 << j;
                adj_list[j] |= (unsigned __int128) 1 << i;
            }
    }

    auto [max_clique_size, clique_mask] = bron_kerbosch(n, adj_list);
    cout << max_clique_size << "\n";
    for (int i = 0; i < n; i++)
        if ((clique_mask >> i) & 1) cout << i + 1 << " ";
}
