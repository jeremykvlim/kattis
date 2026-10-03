#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Fraction : array<T, 2> {
    using F = array<T, 2>;

    bool reduced;

    Fraction() = default;
    Fraction(T n, T d, bool reduced = false) : F{n, d}, reduced(reduced) {
        if (!reduced) reduce();
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

        if (!numer() && denom()) denom() = 1;
        else if (numer() && !denom()) numer() = numer() < 0 ? -1 : 1;
        else if (numer() && denom() && abs(numer()) != 1 && denom() != 1) {
            T g = __gcd(abs(numer()), denom());
            numer() /= g;
            denom() /= g;
        }
        reduced = true;
    }

    bool operator<(const Fraction &f) const {
        return numer() * f.denom() < f.numer() * denom();
    }

    bool operator<(const T &v) const {
        return numer() < v * denom();
    }

    bool operator>(const Fraction &f) const {
        return numer() * f.denom() > f.numer() * denom();
    }

    bool operator>(const T &v) const {
        return numer() > v * denom();
    }

    bool operator==(const Fraction &f) const {
        return numer() == f.numer() && denom() == f.denom();
    }

    bool operator==(const T &v) const {
        return numer() == v * denom();
    }

    bool operator!=(const Fraction &f) const {
        return numer() != f.numer() || denom() != f.denom();
    }

    bool operator!=(const T &v) const {
        return numer() != v * denom();
    }

    bool operator<=(const Fraction &f) const {
        return *this < f || *this == f;
    }

    bool operator<=(const T &v) const {
        return numer() <= v * denom();
    }

    bool operator>=(const Fraction &f) const {
        return *this > f || *this == f;
    }

    bool operator>=(const T &v) const {
        return numer() >= v * denom();
    }

    Fraction operator+(const Fraction &f) const {
        return {numer() * f.denom() + f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator+(const T &v) const {
        return {numer() + v * denom(), denom(), reduced};
    }

    Fraction & operator+=(const Fraction &f) {
        numer() = numer() * f.denom() + f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator+=(const T &v) {
        numer() += v * denom();
        if (!reduced) reduce();
        return *this;
    }

    Fraction operator-(const Fraction &f) const {
        return {numer() * f.denom() - f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator-(const T &v) const {
        return {numer() - v * denom(), denom(), reduced};
    }

    Fraction & operator-=(const Fraction &f) {
        numer() = numer() * f.denom() - f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator-=(const T &v) {
        numer() -= v * denom();
        if (!reduced) reduce();
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
        T fn = f.numer(), fd = f.denom();
        numer() *= fd;
        denom() *= fn;
        reduce();
        return *this;
    }

    Fraction & operator/=(const T &v) {
        denom() *= v;
        reduce();
        return *this;
    }

    friend Fraction operator+(const T &v, const Fraction &f) {
        return f + v;
    }

    friend Fraction operator-(const T &v, const Fraction &f) {
        return {v * f.denom() - f.numer(), f.denom()};
    }

    friend Fraction operator*(const T &v, const Fraction &f) {
        return f * v;
    }

    friend Fraction operator/(const T &v, const Fraction &f) {
        return {v * f.denom(), f.numer()};
    }

    friend bool operator<(const T &v, const Fraction &f) {
        return v * f.denom() < f.numer();
    }

    friend bool operator>(const T &v, const Fraction &f) {
        return v * f.denom() > f.numer();
    }

    friend bool operator==(const T &v, const Fraction &f) {
        return v * f.denom() == f.numer();
    }

    friend bool operator!=(const T &v, const Fraction &f) {
        return v * f.denom() != f.numer();
    }

    friend bool operator<=(const T &v, const Fraction &f) {
        return v * f.denom() <= f.numer();
    }

    friend bool operator>=(const T &v, const Fraction &f) {
        return v * f.denom() >= f.numer();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long k, a, b, c;
    cin >> k >> a >> b >> c;

    vector<long long> v(k + 1, 0);
    auto calc = [&](long long n, long long m) -> long long {
        return v[m] - a * m + v[n - 1 - m] - b * (n - 1 - m);
    };

    for (int n = 1, m = 0; n <= k; n++) {
        m = min(m, n - 1);

        auto v1 = calc(n, m);
        while (m + 1 <= n - 1) {
            auto v2 = calc(n, m + 1);
            if (v1 <= v2) {
                v1 = v2;
                m++;
            } else break;
        }
        v[n] = v1 + c;
    }

    Fraction<long long> f(v[k], k);
    cout << f.numer() << "/" << f.denom();
}
