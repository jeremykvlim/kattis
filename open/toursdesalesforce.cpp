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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T held_karp(int n, const vector<vector<T>> &dist, int src = 0) {
    T inf = numeric_limits<T>::max() / 4;
    vector<vector<T>> dp(1 << n, vector<T>(n, inf));
    vector<vector<int>> prev(1 << n, vector<int>(n, -1));

    for (int i = 0; i < n; i++) dp[1 << i][i] = dist[src][i + 1];

    for (int m1 = 1; m1 < 1 << n; m1++)
        for (int i = 0; i < n; i++)
            if ((m1 >> i) & 1)
                if (dp[m1][i] != inf)
                    for (int m2 = m1 ^ ((1 << n) - 1); m2; m2 &= m2 - 1) {
                        int j = countr_zero((unsigned) m2), m3 = m1 | (1 << j);
                        if (dp[m3][j] > dp[m1][i] + dist[i + 1][j + 1]) {
                            dp[m3][j] = dp[m1][i] + dist[i + 1][j + 1];
                            prev[m3][j] = i;
                        }
                    }

    T len = inf;
    int j = -1;
    for (int i = 0; i < n; i++)
        if (len > dp.back()[i] + dist[i + 1][src]) {
            len = dp.back()[i] + dist[i + 1][src];
            j = i;
        }
    return len;
}

template <typename T>
pair<vector<int>, T> jonker_volgenant(const vector<vector<T>> &C) {
    int n = C.size(), m = C[0].size();

    vector<T> dist(m), potential(m);
    vector<int> row_match(n, -1), col_match(m, -1), cols(m), prev(m);
    iota(cols.begin(), cols.end(), 0);
    T d = 0;
    for (int i = 0, c1 = -1, temp = 0; i < n; i++) {
        for (int c = 0; c < m; c++) {
            dist[c] = C[i][c] - potential[c];
            prev[c] = i;
        }

        int s = 0, t = 0;
        for (;;) {
            if (s == t) {
                temp = s;
                d = dist[cols[t++]];
                for (int j = t; j < m; j++) {
                    c1 = cols[j];
                    if (d < dist[c1]) continue;
                    if (d > dist[c1]) {
                        d = dist[c1];
                        t = s;
                    }
                    cols[j] = exchange(cols[t++], c1);
                }

                for (int j = s; j < t; j++)
                    if (!~col_match[c1 = cols[j]]) goto done;
            }

            int c2 = cols[s++], r = col_match[c2];
            for (int j = t; j < m; j++) {
                c1 = cols[j];
                if (dist[c1] > C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d) {
                    dist[c1] = C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d;
                    prev[c1] = r;

                    if (dist[c1] == d) {
                        if (!~col_match[c1]) goto done;
                        cols[j] = exchange(cols[t++], c1);
                    }
                }
            }
        }
        done:;

        for (int j = 0; j < temp; j++) potential[cols[j]] += dist[cols[j]] - d;
        for (int r = -1; r != i;) {
            r = col_match[c1] = prev[c1];
            swap(c1, row_match[r]);
        }
    }

    T cost = 0;
    for (int i = 0; i < n; i++) cost += C[i][row_match[i]];
    return {row_match, cost};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d;
    cin >> d;

    auto len = [&](const vector<Point<int>> &points) -> double {
        int n = points.size();
        vector<vector<double>> dist(n, vector<double>(n, 0));
        for (int i = 1; i < n; i++) dist[0][i] = dist[i][0] = euclidean_dist(points[0], points[i]);
        for (int i = 1; i < n; i++)
            for (int j = 1; j < n; j++) dist[i][j] = euclidean_dist(points[i], points[j]);
        return held_karp(n - 1, dist);
    };

    double prior = 0;
    vector<vector<Point<int>>> districts(d);
    for (int i = 0; i < d; i++) {
        int n;
        cin >> n;

        districts[i].resize(n);
        for (auto &[x, y] : districts[i]) cin >> x >> y;
        prior += len(districts[i]);
    }

    int m = d / 2;
    vector<int> l(m), r(m);
    iota(l.begin(), l.end(), 0);
    iota(r.begin(), r.end(), m);
    vector<vector<double>> cost(m, vector<double>(m, 0));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++) {
            vector<Point<int>> points;
            for (auto &p : districts[r[i]]) points.emplace_back(p);
            for (auto &p : districts[l[j]]) points.emplace_back(p);
            cost[i][j] = len(points);
        }
    cout << fixed << setprecision(2) << prior << " " << jonker_volgenant(cost).second;
}
