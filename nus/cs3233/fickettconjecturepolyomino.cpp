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

    int t;
    cin >> t;

    while (t--) {
        int R, C;
        cin >> R >> C;

        vector<string> grid(R);
        for (auto &row : grid) cin >> row;

        int u = R, d = 0, l = C, r = 0;
        for (int i = 0; i < R; i++) {
            int j = grid[i].find('#');
            if (j != string::npos) {
                u = min(u, i);
                d = i;
                l = min(l, j);
                r = max(r, (int) grid[i].rfind('#'));
            }
        }

        vector<string> cropped(d - u + 1);
        for (int i = u; i <= d; i++) cropped[i - u] = grid[i].substr(l, r - l + 1);
        grid = cropped;
        R = grid.size();
        C = grid[0].size();

        auto build = [&]() {
            vector<array<int, 4>> A(grid.size());
            for (int i = 0; i < grid.size(); i++) {
                A[i] = {(int) grid[i].find('#'), (int) grid[i].rfind('#'), 1, 0};
                if (i > 1) {
                    A[i - 1][2] = max({A[i - 2][0], A[i - 1][0], A[i][0]}) + 1;
                    A[i - 1][3] = min({A[i - 2][1], A[i - 1][1], A[i][1]}) - 1;
                }
            }
            return A;
        };
        auto A = build();
        Fraction<int> f(0, 1);
        for (int _ = 0; _ < 4; _++) {
            auto A1 = build();
            int R1 = A1.size(), C1 = grid[0].size();
            for (int dr = 1 - R1; dr < R; dr++)
                for (int dc = 1 - C1; dc < C; dc++) {
                    int numer = 0, denom = 0;
                    for (int i = max(0, dr); i < min(R, dr + R1); i++) {
                        int j = i - dr;
                        auto border = [&](const auto &a, int l, int r) {
                            return max(0, min(a[1], r) - max(a[0], l) + 1) - max(0, min(a[3], r) - max(a[2], l) + 1);
                        };
                        numer += border(A[i], A1[j][0] + dc, A1[j][1] + dc);
                        denom += border(A1[j], A[i][0] - dc, A[i][1] - dc);
                    }
                    if (denom) f = max(f, Fraction{numer, denom, false});
                }

            vector<string> g(grid[0].size(), string(grid.size(), '.'));
            for (int i = 0; i < grid.size(); i++)
                for (int j = 0; j < grid[0].size(); j++) g[j][grid.size() - 1 - i] = grid[i][j];
            grid = g;
        }
        f.reduce();
        cout << f.numer() << "/" << f.denom() << "\n";
    }
}
