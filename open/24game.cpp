#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Fraction {
    T numer, denom;

    Fraction() {}
    Fraction(T n, T d) : numer(n), denom(d) {
        reduce();
    }

    void reduce() {
        if (denom < 0) {
            numer *= -1;
            denom *= -1;
        }

        T g = __gcd(abs(numer), denom);
        if (g) {
            numer /= g;
            denom /= g;
        }
    }

    bool operator<(const Fraction &f) const {
        return numer * f.denom < f.numer * denom;
    }

    bool operator>(const Fraction &f) const {
        return numer * f.denom > f.numer * denom;
    }

    bool operator==(const Fraction &f) const {
        return numer == f.numer && denom == f.denom;
    }

    bool operator!=(const Fraction &f) const {
        return numer != f.numer || denom != f.denom;
    }

    bool operator<=(const Fraction &f) const {
        return *this < f || *this == f;
    }

    bool operator>=(const Fraction &f) const {
        return *this > f || *this == f;
    }

    Fraction operator+(const Fraction &f) const {
        return {numer * f.denom + f.numer * denom, denom * f.denom};
    }

    Fraction operator+(const T &v) const {
        return {numer + v * denom, denom};
    }

    Fraction & operator+=(const Fraction &f) {
        numer = numer * f.denom + f.numer * denom;
        denom *= f.denom;
        reduce();
        return *this;
    }

    Fraction & operator+=(const T &v) {
        numer += v * denom;
        reduce();
        return *this;
    }

    Fraction operator-(const Fraction &f) const {
        return {numer * f.denom - f.numer * denom, denom * f.denom};
    }

    Fraction operator-(const T &v) const {
        return {numer - v * denom, denom};
    }

    Fraction & operator-=(const Fraction &f) {
        numer = numer * f.denom - f.numer * denom;
        denom *= f.denom;
        reduce();
        return *this;
    }

    Fraction & operator-=(const T &v) {
        numer -= v * denom;
        reduce();
        return *this;
    }

    Fraction operator*(const Fraction &f) const {
        return {numer * f.numer, denom * f.denom};
    }

    Fraction operator*(const T &v) const {
        return {numer * v, denom};
    }

    Fraction & operator*=(const Fraction &f) {
        numer *= f.numer;
        denom *= f.denom;
        reduce();
        return *this;
    }

    Fraction & operator*=(const T &v) {
        numer *= v;
        reduce();
        return *this;
    }

    Fraction operator/(const Fraction &f) const {
        return {numer * f.denom, denom * f.numer};
    }

    Fraction operator/(const T &v) const {
        return {numer, denom * v};
    }

    Fraction & operator/=(const Fraction &f) {
        numer *= f.denom;
        denom *= f.numer;
        reduce();
        return *this;
    }

    Fraction & operator/=(const T &v) {
        denom *= v;
        reduce();
        return *this;
    }

    struct Hash {
        size_t operator()(Fraction<T> f) const {
            auto h = 0ULL;
            h ^= hash<T>()(f.numer) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(f.denom) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C, t;
    cin >> C >> t;

    vector<int> c(C);
    for (int &ci : c) cin >> ci;
    sort(c.begin(), c.end());

    vector<unordered_map<Fraction<int>, string, Fraction<int>::Hash>> memo(1 << C);
    auto dp = [&](auto &&self, int m1) {
        if (!memo[m1].empty()) return memo[m1];

        unordered_map<Fraction<int>, string, Fraction<int>::Hash> solutions;
        if (__builtin_popcount(m1) == 1) {
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
                        if (f2.numer && !solutions.count({f1 / f2})) solutions[{f1 / f2}] = "(" + s1 + "/" + s2 + ")";
                        if (f1.numer && !solutions.count({f2 / f1})) solutions[{f2 / f1}] = "(" + s2 + "/" + s1 + ")";
                    }
            }
        }
        return memo[m1] = solutions;
    };
    cout << dp(dp, (1 << C) - 1)[{t, 1}];
}
