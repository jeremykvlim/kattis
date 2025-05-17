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

        vector<vector<bool>> cell(R, vector<bool>(C, false));
        for (int r = 0; r < R; r++)
            for (int c = 0; c < C; c++) cell[r][c] = grid[r][c] == '#';

        vector<pair<int, int>> borders;
        vector<int> dr{1, 0, -1, 0, 1, -1, -1, 1}, dc{0, 1, 0, -1, 1, 1, -1, -1};
        for (int row = 0; row < R; row++)
            for (int col = 0; col < C; col++)
                if (cell[row][col])
                    for (int k = 0; k < 8; k++) {
                        int r = row + dr[k], c = col + dc[k];
                        if (!(0 <= r && r < R && 0 <= c && c < C) || !cell[r][c]) {
                            borders.emplace_back(row, col);
                            break;
                        }
                    }

        Fraction<int> f(1, 1);
        for (int rotation = 0; rotation < 4; rotation++) {
            int C_r = rotation & 1 ? R : C, R_r = rotation & 1 ? C : R;

            auto rotate = [&](int r, int c) -> pair<int, int> {
                if (!rotation) return {r, c};
                if (rotation == 1) return {c, R - 1 - r};
                if (rotation == 2) return {R - 1 - r, C - 1 -c};
                return {C - 1 - c, r};
            };
          
            vector<vector<bool>> cell_r(R_r, vector<bool>(C_r, false));
            for (int r = 0; r < R; r++)
                for (int c = 0; c < C; c++)
                    if (cell[r][c]) {
                        auto [row, col] = rotate(r, c);
                        cell_r[row][col] = true;
                    }
          
            vector<pair<int, int>> borders_r;
            for (auto [r, c] : borders) borders_r.emplace_back(rotate(r, c));

            for (int drow = 1 - R_r; drow <= R - 1; drow++)
                for (int dcol = 1 - C_r; dcol <= C - 1; dcol++) {
                    int n = 0, d = 0;
                    for (auto [row, col] : borders) {
                        int i = row - drow, j = col - dcol;
                        if (0 <= i && i < R_r && 0 <= j && j < C_r && cell_r[i][j]) n++;
                    }
                    if (!n) continue;

                    for (auto [row, col] : borders_r) {
                        int i = row + drow, j = col + dcol;
                        if (0 <= i && i < R && 0 <= j && j < C && cell[i][j]) d++;
                    }
                    if (!d) continue;

                    Fraction<int> temp(n, d);
                    if (f < temp) f = temp;
                }
        }
        cout << f.numer() << "/" << f.denom() << "\n";
    }
}
