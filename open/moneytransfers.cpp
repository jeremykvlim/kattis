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

    Point operator!() const {
        return {y, x};
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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull : deque<Point<T>> {
    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (this->size() > 1 && violates((*this)[1], (*this)[0], p)) this->pop_front();
        this->emplace_front(p);
    }
};

template <typename T>
struct AffineFunction {
    T m, c;

    AffineFunction(T m = 0, T c = 0) : m(m), c(c) {}

    T operator()(T x) {
        return m * x + c;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p, x, y;
    cin >> n >> p >> x >> y;

    vector<array<int, 3>> edges(p);
    for (auto &[a, b, c] : edges) cin >> a >> b >> c;

    int m;
    cin >> m;

    vector<bool> owned(n + 1, false);
    while (m--) {
        int i;
        cin >> i;

        owned[i] = true;
    }

    vector<long long> dp1(n + 1, 1e18), dp2(n + 1, 1e18), temp1(n + 1), temp2(n + 1), cost1(n, 1e18), cost2(n, 1e18);
    dp1[x] = 0;
    for (int i = 0; i < n - 1; i++) {
        fill(temp1.begin(), temp1.end(), 1e18);
        fill(temp2.begin(), temp2.end(), 1e18);
        for (auto [u, v, w] : edges) {
            if (owned[v]) temp1[v] = min(temp1[v], dp1[u] + w);
            else temp2[v] = min(temp2[v], dp1[u] + w);
            if (owned[u]) temp1[u] = min(temp1[u], dp1[v] + w);
            else temp2[u] = min(temp2[u], dp1[v] + w);

            temp2[v] = min(temp2[v], dp2[u] + w);
            temp2[u] = min(temp2[u], dp2[v] + w);
        }

        cost1[i + 1] = temp1[y];
        cost2[i + 1] = temp2[y];
        dp1 = temp1;
        dp2 = temp2;
    }

    if (all_of(cost1.begin() + 1, cost1.end(), [&](auto c) { return c == 1e18; })) {
        cout << "Impossible";
        exit(0);
    }

    if (all_of(cost2.begin() + 1, cost2.end(), [&](auto c) { return c == 1e18; })) {
        cout << "Infinity";
        exit(0);
    }

    auto cht = [&](const auto &cost) -> pair<vector<AffineFunction<long long>>, vector<long long>> {
        MonotonicHull<long long, 1, true> hull;
        for (int i = n - 1; i; i--)
            if (cost[i] != 1e18) hull.add(Point<long long>(i, cost[i]));

        vector<AffineFunction<long long>> functions;
        for (auto it = hull.rbegin(); it != hull.rend(); it++) functions.emplace_back(it->x, it->y);

        vector<long long> threshold(functions.size(), 0);
        for (int i = 1; i < functions.size(); i++) {
            auto [m1, c1] = functions[i - 1];
            auto [m2, c2] = functions[i];
            threshold[i] = ceil((double) (c2 - c1) / (m1 - m2));
        }
        return {functions, threshold};
    };
    auto [f1, t1] = cht(cost1);
    auto [f2, t2] = cht(cost2);

    auto fee = -1LL, l = 1LL;
    for (int i = 0, j = 0; l != LLONG_MAX;) {
        for (; i + 1 < t1.size() && t1[i + 1] <= l; i++);
        for (; j + 1 < t2.size() && t2[j + 1] <= l; j++);

        auto [m1, c1] = f1[i];
        auto [m2, c2] = f2[j];
        auto dm = m1 - m2, dc = c2 - c1, r = min((i + 1 < t1.size()) ? t1[i + 1] : LLONG_MAX, (j + 1 < t2.size()) ? t2[j + 1] : LLONG_MAX) - 1, temp = r;

        bool valid = true;
        if (!dm) {
            if (dc <= 0) valid = false;
        } else if (dm > 0) {
            if (dc <= 0) valid = false;
            else r = min(r, (dc - 1) / dm);
        } else l = max(l, dc / dm + 1);

        if (valid && l <= r) {
            if (r + 1 == LLONG_MAX) {
                cout << "Infinity";
                exit(0);
            }
            fee = max(fee, r);
        }
        l = temp + 1;
    }

    if (fee < 1) cout << "Impossible";
    else cout << fee;
}
