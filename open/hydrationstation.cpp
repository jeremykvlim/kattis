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

    int n;
    cin >> n;

    auto sum = 0LL;
    int b_max = 0;
    vector<int> b(n);
    for (int &bi : b) {
        cin >> bi;

        sum += bi;
        b_max = max(b_max, bi);
    }

    vector<int> freq(b_max + 1, 0), count(b_max + 1, 0), suff(b_max + 2, 0);
    for (int bi : b) freq[bi]++;
    for (int k = 1; k <= b_max; k++)
        for (int i = k; i <= b_max; i += k) count[k] += freq[i];
    for (int i = b_max; i; i--) suff[i] = suff[i + 1] + freq[i];

    auto C = LLONG_MAX, D = 0LL;
    for (int k = 1; k <= b_max; k++) {
        long long temp = n;
        for (int i = k; i < b_max; i += k) temp += suff[i + 1];

        if (C > temp * (k + 1)) {
            C = temp * (k + 1);
            D = k + 1;
        }
    }

    int u = 0, v = 1, x = 1, y = b_max;
    Fraction<long long> cost{C, 1}, dispense{D, 1};
    while (x <= y && max((long long) n, x * sum / y) < cost - sum) {
        if (count[y]) {
            Fraction<long long> c{(long long) n * (x + y), x, true};
            if (cost > c) {
                cost = c;
                dispense = {x + y, x, true};
            }
            n += count[y];
        }

        int k = (b_max + v) / y;
        tie(u, v, x, y) = tuple{x, y, k * x - u, k * y - v};
    }
    cout << dispense.numer() << "/" << dispense.denom();
}