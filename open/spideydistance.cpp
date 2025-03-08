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

    struct FractionHash {
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

    long long t, s;
    cin >> t >> s;

    long long n = 2 * min(t, s) * (min(t, s) + 1) + 1, d = 2 * s * (s + 1) + 1;
    for (int i = s - 2, j = max(0, (int) t - (int) s); i > 0; i -= 3) {
        d += 4 * i;
        if (j) {
            n += 4 * i;
            j--;
        }
    }

    Fraction<long long> f(n, d);
    cout << f.numer;
    if (f.denom != 1) cout << "/" << f.denom;
}
