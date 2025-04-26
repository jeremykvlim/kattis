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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C, t;
    cin >> C >> t;

    vector<int> c(C);
    for (int &ci : c) cin >> ci;
    sort(c.begin(), c.end());

    vector<unordered_map<Fraction<int>, string, Hash>> memo(1 << C);
    auto dp = [&](auto &&self, int m1) {
        if (!memo[m1].empty()) return memo[m1];

        unordered_map<Fraction<int>, string, Hash> solutions;
        if (popcount((unsigned) m1) == 1) {
            for (int i = 0; i < C; i++)
                if ((m1 >> i) & 1) {
                    solutions[{c[i], 1}] = to_string(c[i]);
                    break;
                }
        } else {
            for (int m2 = (m1 - 1) & m1; m2; --m2 &= m1) {
                if (m1 == m2) continue;

                auto left = self(self, m2), right = self(self, m1 ^ m2);
                for (auto [f1, s1] : left)
                    for (auto [f2, s2] : right) {
                        if (!solutions.count({f1 + f2})) solutions[{f1 + f2}] = "(" + s1 + "+" + s2 + ")";
                        if (!solutions.count({f1 - f2})) solutions[{f1 - f2}] = "(" + s1 + "-" + s2 + ")";
                        if (!solutions.count({f2 - f1})) solutions[{f2 - f1}] = "(" + s2 + "-" + s1 + ")";
                        if (!solutions.count({f1 * f2})) solutions[{f1 * f2}] = "(" + s1 + "*" + s2 + ")";
                        if (f2.numer() && !solutions.count({f1 / f2})) solutions[{f1 / f2}] = "(" + s1 + "/" + s2 + ")";
                        if (f1.numer() && !solutions.count({f2 / f1})) solutions[{f2 / f1}] = "(" + s2 + "/" + s1 + ")";
                    }
            }
        }
        return memo[m1] = solutions;
    };
    cout << dp(dp, (1 << C) - 1)[{t, 1}];
}
