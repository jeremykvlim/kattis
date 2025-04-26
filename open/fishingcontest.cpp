#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
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
T manhattan_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

struct FenwickTree {
    int n;
    vector<int> indices;

    void update(int i) {
        for (; i && i < n; i += i & -i) indices.emplace_back(i);
    }

    void pref_sum(int i) {
        for (; i; i &= i - 1) indices.emplace_back(i);
    }

    vector<int> order(int i) {
        pref_sum(i);
        indices.emplace_back(0);
        update(i);
        indices.erase(indices.begin());
        return indices;
    }

    FenwickTree(int n) : n(n) {}
};

struct PersistentSegmentTree {
    struct Segment {
        int value;

        Segment(int v = -1) : value(v) {}

        auto operator+=(const Segment &seg) {
            value = max(value, seg.value);
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n;
    vector<int> roots;
    vector<Segment> ST;
    vector<pair<int, int>> children;

    PersistentSegmentTree(int n) : n(n), roots{0}, ST(1), children{{0, 0}} {}

    int modify(int i, const int &v, const int &pos) {
        roots.emplace_back(modify(roots[i], v, pos, 1, n));
        return roots.size() - 1;
    }

    int modify(int i, const int &v, const int &pos, int tl, int tr) {
        if (tl + 1 == tr) {
            children.emplace_back(0, 0);
            ST.emplace_back(ST[i] + v);
            return ST.size() - 1;
        }

        auto [cl, cr] = children[i];
        int tm = tl + (tr - tl) / 2;
        if (pos < tm) cl = modify(cl, v, pos, tl, tm);
        else cr = modify(cr, v, pos, tm, tr);

        children.emplace_back(cl, cr);
        ST.emplace_back(ST[cl] + ST[cr]);
        return ST.size() - 1;
    }

    Segment range_query(int i, int l, int r) {
        return range_query(roots[i], l, r, 1, n);
    }

    Segment range_query(int i, int l, int r, int tl, int tr) {
        if (!i || r <= tl || tr <= l) return {};
        if (l <= tl && tr <= r) return ST[i];

        auto [cl, cr] = children[i];
        int tm = tl + (tr - tl) / 2;
        return range_query(cl, l, r, tl, tm) + range_query(cr, l, r, tm, tr);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c, K, l, x0, y0;
    cin >> r >> c >> K >> l >> x0 >> y0;

    vector<vector<int>> times(r, vector<int>(c));
    vector<vector<Point<int>>> fish(l + 1);
    for (int x = 0; x < r; x++)
        for (int y = 0; y < c; y++) {
            int t;
            cin >> t;

            times[x][y] = t;
            for (int k = 0; k < K; k++)
                if (t + k <= l) fish[t + k].emplace_back(x, y);
        }

    vector<vector<int>> row(r), col(c);
    row[0] = col[0] = {0};
    for (int x = 1; x < r; x++) {
        FenwickTree fw(r);
        row[x] = fw.order(x);
    }
    for (int y = 1; y < c; y++) {
        FenwickTree fw(c);
        col[y] = fw.order(y);
    }

    vector<vector<pair<Point<int>, int>>> events(l + 1);
    for (int x1 : row[x0])
        for (int y1 : col[y0]) {
            int t = manhattan_dist(Point(x0, y0), Point(x1, y1)) + 1;
            if (t <= l) events[t].emplace_back(Point(x1, y1), 0);
        }

    vector<vector<int>> dp(r, vector<int>(c, -1));
    vector<vector<PersistentSegmentTree>> psts(r, vector<PersistentSegmentTree>(c, PersistentSegmentTree(l + 1)));
    vector<unordered_map<Point<int>, unordered_map<int, int>, Hash>> states(l + 1);

    auto encode = [&](int dx, int dy) {
        return 1 << ((dx + 2) * 5 + dy + 2);
    };

    int m0 = encode(0, 0),
        centre = m0 | encode(1, 0) | encode(0, 1) | encode(-1, 0) | encode(0, -1),
        up = centre | encode(-2, 0) | encode(-1, -1) | encode(-1, 1),
        down = centre | encode(2, 0) | encode(1, -1) | encode(1, 1),
        left = centre | encode(0, -2) | encode(-1, -1) | encode(1, -1),
        right = centre | encode(0, 2) | encode(-1, 1) | encode(1, 1);

    int most = 0;
    for (int t1 = 0; t1 <= l; t1++) {
        for (auto [p, count] : events[t1]) {
            auto [x1, y1] = p;
            if (dp[x1][y1] < count) {
                dp[x1][y1] = count;
                psts[x1][y1].modify(psts[x1][y1].roots.size() - 1, count, t1 + 1);
            }
        }

        for (auto p : fish[t1]) {
            auto [x1, y1] = p;
            int count = -1;
            for (int x2 : row[x1])
                for (int y2 : col[y1]) {
                    int t2 = t1 - manhattan_dist(Point(x1, y1), Point(x2, y2)) + 1;
                    if (t2 > 0) count = max(count, psts[x2][y2].range_query(psts[x2][y2].roots.size() - 1, 1, t2 + 1).value);
                }

            if (~count) states[t1][p][m0] = max(states[t1][p][m0], count + 1);
        }

        for (auto &[p, curr] : states[t1]) {
            auto [x1, y1] = p;
            int m1 = 0;
            for (int dx = -2; dx <= 2; dx++)
                for (int dy = -2; dy <= 2; dy++) {
                    int dt = manhattan_dist(Point(dx, dy)), x2 = x1 + dx, y2 = y1 + dy, t2 = t1 + dt;
                    if (dt <= 2 && 0 <= x2 && x2 < r && 0 <= y2 && y2 < c && t2 - K < times[x2][y2] && times[x2][y2] <= t2) m1 |= encode(dx, dy);
                }

            int m2 = m0 & m1;
            unordered_map<int, int> next;
            for (auto [m3, count] : curr) {
                auto m4 = m1 & m3;
                if ((m4 & m2) != m2) {
                    m4 |= m2;
                    count++;
                }
                next[m4] = max(next[m4], count);
            }
            curr = next;

            for (auto [m3, count] : curr) {
                most = max(most, count);

                if (!m3) {
                    for (int x2 : row[x1])
                        for (int y2 : col[y1]) {
                            int t2 = t1 + manhattan_dist(Point(x1, y1), Point(x2, y2));
                            if (t1 < t2 && t2 <= l) events[t2].emplace_back(Point(x2, y2), count);
                        }

                    if (dp[x1][y1] < count) {
                        dp[x1][y1] = count;
                        psts[x1][y1].modify(psts[x1][y1].roots.size() - 1, count, t1 + 1);
                    }
                } else if (t1 < l) {
                    states[t1 + 1][p][m3] = max(states[t1 + 1][p][m3], count);
                    Point<int> pu = p + Point(-1, 0), pd = p + Point(1, 0), pl = p + Point(0, -1), pr = p + Point(0, 1);
                    int m4 = (m3 & up) << 5, m5 = (m3 & down) >> 5, m6 = (m3 & left) << 1, m7 = (m3 & right) >> 1;
                    for (auto [q, mask] : vector<pair<Point<int>, int>>{{pu, m4}, {pd, m5}, {pl, m6}, {pr, m7}})
                        if (0 <= q.x && q.x < r && 0 <= q.y && q.y < c) states[t1 + 1][q][mask] = max(states[t1 + 1][q][mask], count);
                }
            }
        }

        if (t1 + 2 <= l) states[t1 + 2].clear();
    }
    cout << most;
}
