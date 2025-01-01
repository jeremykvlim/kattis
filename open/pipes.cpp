#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-8;
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

    struct Hash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

Point<int> tile(int l, int m, int n) {
    if (l <= m) return {l - 1, 0};
    if (l <= m + n) return {m - 1, l - m - 1};
    if (l <= 2 * m + n) return {2 * m + n - l, n - 1};
    return {0, 2 * (m + n) - l};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    while (k--) {
        int m, n, l;
        cin >> m >> n >> l;

        vector<int> indices(2 * (m + n) + 1, -1);
        deque<int> locations(2 * l);
        vector<Point<int>> connect(l);
        for (int i = 0; i < l; i++) {
            int s, t;
            cin >> s >> t;

            connect[i] = {s, t};
            locations[2 * i] = s;
            locations[2 * i + 1] = t;
            indices[s] = indices[t] = i;
        }
        sort(locations.begin(), locations.end());

        vector<int> order;
        while (!locations.empty()) {
            if (indices[locations[0]] == indices[locations.back()]) {
                order.emplace_back(indices[locations[0]]);
                locations.pop_front();
                locations.pop_back();
                continue;
            }

            bool change = false;
            for (int i = 1; i < locations.size(); i++)
                if (indices[locations[i - 1]] == indices[locations[i]]) {
                    order.emplace_back(indices[locations[i]]);
                    locations.erase(locations.begin() + i - 1);
                    locations.erase(locations.begin() + i - 1);
                    change = true;
                    break;
                }

            if (!change) break;
        }

        if (!locations.empty()) {
            no:;
            cout << "NO\n";
            continue;
        }

        vector<vector<bool>> visited(m, vector<bool>(n, false));
        for (int i : order) {
            auto [r1, c1] = tile(connect[i].x, m, n);
            auto [r2, c2] = tile(connect[i].y, m, n);

            if (visited[r1][c1] || visited[r2][c2]) goto no;

            visited[r1][c1] = visited[r2][c2] = true;
        }

        vector<vector<bool>> border(m, vector<bool>(n, false));
        for (int i = 0; i < m; i++) border[i][0] = border[i][n - 1] = true;
        for (int i = 0; i < n; i++) border[0][i] = border[m - 1][i] = true;

        auto valid = [&](auto p) -> bool {return 0 <= p.x && p.x < m && 0 <= p.y && p.y < n;};
        vector<Point<int>> drdc1{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}, drdc2{{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
        for (int i : order) {
            auto s = tile(connect[i].x, m, n), d = tile(connect[i].y, m, n);

            unordered_map<Point<int>, Point<int>, Point<int>::Hash> prev;
            unordered_map<Point<int>, int, Point<int>::Hash> dist;
            dist[s] = 0;
            stack<Point<int>> st;
            st.emplace(s);
            while (!st.empty()) {
                auto v = st.top();
                st.pop();

                if (v == d) break;

                for (int j = 0; j < 4; j++) {
                    auto u = v + drdc1[j];

                    if (u == d) {
                        dist[u] = dist[v] + 1;
                        prev[u] = v;
                        goto done;
                    }

                    if (!valid(u) || visited[u.x][u.y] || !border[u.x][u.y]) continue;

                    if (!dist.count(u)) {
                        dist[u] = dist[v] + 1;
                        prev[u] = v;
                        st.emplace(u);
                    }
                }
            }
            done:;

            if (!dist.count(d)) goto no;

            for (int j = 0; j < 8; j++) {
                auto v = d + drdc2[j];
                if (valid(v)) border[v.x][v.y] = true;
            }

            while (s != d) {
                d = prev[d];
                visited[d.x][d.y] = true;

                for (int j = 0; j < 8; j++) {
                    auto v = d + drdc2[j];
                    if (valid(v)) border[v.x][v.y] = true;
                }
            }
        }

        cout << "YES\n";
    }
}
