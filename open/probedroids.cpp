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
    static pair<Fraction<T>, Fraction<T>> find_valid_interval(T bound, P &&predicate) {
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
                if (k > numeric_limits<T>::max() >> 1) break;
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
                if (k > numeric_limits<T>::max() >> 1) break;
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
        bool left = f < Fraction{1, 1};
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

long long floor_division_sum(long long n, int a, int c, int d) {
    auto sum = 0LL;
    while (n) {
        if (a >= d) {
            sum += (a / d) *  (n * (n - 1)) / 2;
            a %= d;
        }
        if (c >= d) {
            sum += n * (c / d);
            c %= d;
        }

        auto y_max = a * n + c;
        if (y_max < d) break;
        n = y_max / d;
        c = y_max % d;
        swap(a, d);
    }
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    int q;
    cin >> n >> m >> q;

    auto bound = max(n, m);
    while (q--) {
        long long i;
        cin >> i;

        if (i < m) {
            cout << "1 " << i + 1 << "\n";
            continue;
        }

        if (i > n * (m - 1)) {
            cout << i - n * (m - 1) + 1 << " 1\n";
            continue;
        }

        auto count_leq = [&](const Fraction<long long> &f) {
            auto limit = min(m, ((n - 1) * f.denom() + f.numer() - 1) / f.numer());
            return m - 1 + floor_division_sum(limit, f.numer(), 0, f.denom()) + (m - limit) * (n - 1);
        };

        auto [l, r] = SternBrocotTree::find_valid_interval(bound, [&](const Fraction<long long> &f) -> bool { return count_leq(f) >= i; });
        i += min((n - 1) / r.numer(), (m - 1) / r.denom()) - count_leq(r);
        cout << i * r.numer() + 1 << " " << i * r.denom() + 1 << "\n";
    }
}
