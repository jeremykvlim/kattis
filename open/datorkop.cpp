#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Fraction : array<T, 2> {
    using F = array<T, 2>;

    Fraction() = default;
    Fraction(T n, T d) : F{n, d} {
        reduce();
    }

    T & numer() {
        return (*this)[0];
    }

    T & denom() {
        return (*this)[1];
    }

    const T & numer() const {
        return (*this)[0];
    }

    const T & denom() const {
        return (*this)[1];
    }

    void reduce() {
        if (denom() < 0) {
            numer() *= -1;
            denom() *= -1;
        }

        T g = __gcd(abs(numer()), denom());
        if (g) {
            numer() /= g;
            denom() /= g;
        }
    }

    bool operator<(const Fraction &f) const {
        return numer() * f.denom() < f.numer() * denom();
    }

    bool operator>(const Fraction &f) const {
        return numer() * f.denom() > f.numer() * denom();
    }

    bool operator==(const Fraction &f) const {
        return numer() == f.numer() && denom() == f.denom();
    }

    bool operator!=(const Fraction &f) const {
        return numer() != f.numer() || denom() != f.denom();
    }

    bool operator<=(const Fraction &f) const {
        return *this < f || *this == f;
    }

    bool operator>=(const Fraction &f) const {
        return *this > f || *this == f;
    }

    Fraction operator+(const Fraction &f) const {
        return {numer() * f.denom() + f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator+(const T &v) const {
        return {numer() + v * denom(), denom()};
    }

    Fraction & operator+=(const Fraction &f) {
        numer() = numer() * f.denom() + f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator+=(const T &v) {
        numer() += v * denom();
        reduce();
        return *this;
    }

    Fraction operator-(const Fraction &f) const {
        return {numer() * f.denom() - f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator-(const T &v) const {
        return {numer() - v * denom(), denom()};
    }

    Fraction & operator-=(const Fraction &f) {
        numer() = numer() * f.denom() - f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator-=(const T &v) {
        numer() -= v * denom();
        reduce();
        return *this;
    }

    Fraction operator*(const Fraction &f) const {
        return {numer() * f.numer(), denom() * f.denom()};
    }

    Fraction operator*(const T &v) const {
        return {numer() * v, denom()};
    }

    Fraction & operator*=(const Fraction &f) {
        numer() *= f.numer();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator*=(const T &v) {
        numer() *= v;
        reduce();
        return *this;
    }

    Fraction operator/(const Fraction &f) const {
        return {numer() * f.denom(), denom() * f.numer()};
    }

    Fraction operator/(const T &v) const {
        return {numer(), denom() * v};
    }

    Fraction & operator/=(const Fraction &f) {
        numer() *= f.denom();
        denom() *= f.numer();
        reduce();
        return *this;
    }

    Fraction & operator/=(const T &v) {
        denom() *= v;
        reduce();
        return *this;
    }
};

template <typename T>
Fraction<T> mediant(const Fraction<T> &l, const Fraction<T> &r) {
    return {l.numer() + r.numer(), l.denom() + r.denom()};
}

struct SternBrocotTree {
    template <typename T, typename P>
    static pair<Fraction<T>, Fraction<T>> find_interval(T bound, P &&predicate) {
        Fraction<T> l{0, 1}, r{1, 0};
        for (T k, nl = 0, nr = 0;; nl = 0, nr = 0) {
            auto check_left = [&]() {
                T num = (nl + k) * l.numer() + r.numer(), den = (nl + k) * l.denom() + r.denom();
                if (num > bound || den > bound || !predicate({num, den})) return false;
                return true;
            };

            k = 1;
            while (check_left()) {
                nl += k;
                k <<= 1;
            }

            for (k >>= 1; k; k >>= 1) {
                if (!check_left()) continue;
                nl += k;
            }

            if (nl) {
                r.numer() += nl * l.numer();
                r.denom() += nl * l.denom();
            }

            auto check_right = [&]() {
                T num = l.numer() + (nr + k) * r.numer(), den = l.denom() + (nr + k) * r.denom();
                if (num > bound || den > bound || predicate({num, den})) return false;
                return true;
            };

            k = 1;
            while (check_right()) {
                nr += k;
                k <<= 1;
            }

            for (k >>= 1; k; k >>= 1) {
                if (!check_right()) continue;
                nr += k;
            }

            if (nr) {
                l.numer() += nr * r.numer();
                l.denom() += nr * r.denom();
            }

            if (!nl && !nr) return {l, r};
        }
    }

    template <typename T>
    static vector<pair<char, T>> path_to_target(const Fraction<T> &f) {
        vector<pair<char, T>> path;
        bool left = f < Fraction<T>{1, 1};
        Fraction<T> l{0, 1}, r{1, 0};
        while (f != mediant(l, r)) {
            if (left) {
                T num = f.denom() * (l.numer() + r.numer()) - f.numer() * (l.denom() + r.denom()), den = f.numer() * l.denom() - f.denom() * l.numer(),
                        n = (num + den - 1) / den;
                path.emplace_back('L', n);
                r.numer() += n * l.numer();
                r.denom() += n * l.denom();
            } else {
                T num = f.numer() * (l.denom() + r.denom()) - f.denom() * (l.numer() + r.numer()), den = f.denom() * r.numer() - f.numer() * r.denom(),
                        n = (num + den - 1) / den;
                path.emplace_back('R', n);
                l.numer() += n * r.numer();
                l.denom() += n * r.denom();
            }
            left = !left;
        }
        return path;
    }

    template <typename T>
    static pair<Fraction<T>, Fraction<T>> interval_from_path(const vector<pair<char, T>> &path) {
        Fraction<T> l{0, 1}, r{1, 0};
        for (auto [dir, n] : path)
            if (dir == 'L') {
                r.numer() += n * l.numer();
                r.denom() += n * l.denom();
            } else {
                l.numer() += n * r.numer();
                l.denom() += n * r.denom();
            }
        return {l, r};
    }

    template <typename T>
    static Fraction<T> lca(const Fraction<T> &f1, const Fraction<T> &f2) {
        auto path1 = path_to_target(f1), path2 = path_to_target(f2);
        vector<pair<char, T>> lca_path;
        for (int i = 0; i < min(path1.size(), path2.size()); i++) {
            if (path1[i].first == path2[i].first) {
                if (path1[i].second == path2[i].second) lca_path.emplace_back(path1[i]);
                else {
                    lca_path.emplace_back(path1[i].first, min(path1[i].second, path2[i].second));
                    break;
                }
            } else break;
        }
        auto [l, r] = interval_from_path(lca_path);
        return mediant(l, r);
    }

    template <typename T>
    static Fraction<T> ancestor(const Fraction<T> &f, int k) {
        auto path = path_to_target(f);
        Fraction<T> l{0, 1}, r{1, 0};
        for (auto [dir, n] : path) {
            if (dir == 'L') {
                T m = min(n, k);
                r.numer() += m * l.numer();
                r.denom() += m * l.denom();
                k -= m;
            } else {
                T m = min(n, k);
                l.numer() += m * r.numer();
                l.denom() += m * r.denom();
                k -= m;
            }
            if (!k) return mediant(l, r);
        }
        if (k) return {0, 0};
        return mediant(l, r);
    }
};

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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

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

    bool query(const Point<T> &p, const T &scale) const {
        int l = 0, r = this->size() - 1, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (scale * (*this)[m].x >= p.x) l = m;
            else r = m;
        }

        auto left = (*this)[l], right = (*this)[l + 1], v = left - right;
        return cross(v, p) >= scale * cross(v, right);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long x, a, y, b, n;
    cin >> x >> a >> y >> b >> n;

    auto feasible = [&](auto m) {
        auto blocks = (m + b - 1) / b;
        if (y >= n * blocks) return true;

        auto path = SternBrocotTree::path_to_target(Fraction{b, a});
        Point<long long> l{1, 0}, r{0, 1};
        vector<Point<long long>> dirs{l, r};
        if (a > b) dirs.emplace_back(l);
        for (int i = 0; i < path.size(); i++) {
            auto [dir, k] = path[i];
            if (dir == 'L') r += l * k;
            else l += r * k;
            if (i + 1 < path.size()) dirs.emplace_back(dir == 'L' ? r : l);
        }
        dirs.emplace_back(l + r);

        auto delta = [&](const Point<long long> &p) {
            return cross({a, b}, p);
        };

        vector<Point<long long>> points{{0, -((m + a - 1) / a)}};
        auto add = [&](const Point<long long> &p) {
            auto k = (blocks - 1 - points.back().x) / p.x, d = delta(p);
            if (d > 0) k = min(k, (-m - delta(points.back())) / d);
            if (k) points.emplace_back(points.back() + p * k);
        };

        for (int i = 2; i + 1 < dirs.size(); i += 2)
            while (delta(points.back() + dirs[i + 1]) <= -m) {
                auto d = dirs[i + 1] - dirs[i] * ((m + delta(points.back() + dirs[i + 1])) / delta(dirs[i]));
                if (!(0 < d.x && d.x <= blocks - 1 - points.back().x)) break;
                add(d);
            }
        add(dirs.back());

        for (int i = (dirs.size() - 2) | 1; i > 2; i -= 2)
            while (points.back().x + dirs[i - 1].x <= blocks - 1) add(dirs[i - 1] + dirs[i] * ((blocks - 1 - points.back().x - dirs[i - 1].x) / dirs[i].x));

        MonotonicHull<__int128, -1, true> mh;
        for (auto p : points) mh.add(Point<__int128>{p.x, -p.y});
        mh.add(Point<__int128>{blocks, 0});
        return mh.query({y, x}, n);
    };

    long long l = 0, r = ((x * a + y * b) / n) + 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (feasible(m)) l = m;
        else r = m;
    }
    cout << l;
}