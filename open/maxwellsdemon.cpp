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
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(n, m);
    if ((b - a) % g) return {0, -1};

    if (m > n) {
        swap(a, b);
        swap(n, m);
    }

    a %= n;
    b %= m;
    T lcm = n / g * m;

    n /= g;
    m /= g;
    auto [x, y] = bezout(n, m);
    T r = ((__int128) a * m * y + (__int128) b * n * x) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}

template <int S>
struct GaloisField2Matrix {
    int r, c;
    vector<bitset<S>> mat;

    GaloisField2Matrix(int rows, bool v = false) : r(rows), c(S), mat(rows, v ? ~bitset<S>() : bitset<S>()) {}
    GaloisField2Matrix(const vector<bitset<S>> &mat) : r(mat.size()), c(S), mat(mat) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <int S>
vector<int> rref(GaloisField2Matrix<S> &matrix) {
    int n = matrix.r, m = matrix.c;

    int rank = 0;
    vector<int> pivot_cols;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (; pivot < n && !matrix[pivot][c]; pivot++);

        if (pivot == n) continue;
        swap(matrix[pivot], matrix[rank]);
        pivot_cols.emplace_back(c);

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) matrix[i] ^= matrix[rank];

        rank++;
    }
    return pivot_cols;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w, h, d, R, B;
    cin >> w >> h >> d >> R >> B;
    int p = R + B;

    bitset<201> cross;
    vector<array<int, 4>> particles(p);
    for (int i = 0; i < R; i++) {
        int px, py, vx, vy;
        cin >> px >> py >> vx >> vy;

        px += w;
        if (px > w) {
            cross[i] = true;
            px = 2 * w - px;
            vx = -vx;
        }
        particles[i] = {px, py, vx, vy};
    }
    for (int i = R; i < p; i++) {
        int px, py, vx, vy;
        cin >> px >> py >> vx >> vy;

        px += w;
        if (px <= w) cross[i] = true;
        else {
            px = 2 * w - px;
            vx = -vx;
        }
        particles[i] = {px, py, vx, vy};
    }

    if (cross.none()) {
        cout << 0;
        exit(0);
    }

    for (int i = 0; i < p; i++)
        if (!particles[i][2] && cross[i]) {
            cout << "impossible";
            exit(0);
        }

    vector<int> count(p, 0);
    vector<array<array<Fraction<int>, 2>, 2>> solutions(p);
    for (int i = 0; i < p; i++) {
        auto [px, py, vx, vy] = particles[i];
        if (!vx) continue;

        int avx = abs(vx), avy = abs(vy), l = (!vy ? avx : lcm(avx, avy)),
                dx = w - px, n = 2 * w * (l / avx), a = (dx * (l / vx) + n) % n;
        if (!vy) {
            if (py == d) {
                solutions[i][0][0] = {a, l};
                solutions[i][0][1] = {n, l};
                count[i]++;
            }
            continue;
        }

        array<int, 2> residues;
        residues[0] = d;
        residues[1] = !d ? 0 : 2 * h - d;
        for (int k = 0; k < 2; k++) {
            if (k && residues[0] == residues[1]) break;

            int dy = residues[k] - py, m = 2 * h * (l / avy), b = (dy * (l / vy) + m) % m;
            auto [r, lcm] = chinese_remainder_theorem(a, n, b, m);
            if (lcm != -1) {
                solutions[i][count[i]][0] = {r, l};
                solutions[i][count[i]][1] = {lcm, l};
                count[i]++;
            }
        }
    }

    for (int i = 0; i < p; i++)
        if (cross[i] && !count[i]) {
            cout << "impossible";
            exit(0);
        }

    map<Fraction<int>, bitset<201>> sweep;
    for (int i = 0; i < p; i++)
        for (int j = 0; j < count[i]; j++) {
            auto f = solutions[i][j][0];
            if (!solutions[i][j][1].numer()) sweep[f][i] = true;
            else
                for (int _ = 0; _ < p; _++) {
                    sweep[f][i] = true;
                    f += solutions[i][j][1];
                }
        }

    if (sweep.empty()) {
        cout << "impossible";
        exit(0);
    }

    vector<Fraction<int>> times;
    vector<bitset<201>> rows;
    for (auto [f, bs] : sweep) {
        times.emplace_back(f);
        rows.emplace_back(bs);
    }

    auto reachable = [&](int n) {
        GaloisField2Matrix<201> m({rows.begin(), rows.begin() + n + 1});
        auto pivot_cols = rref(m);

        auto temp = cross;
        for (int r = 0; r < pivot_cols.size(); r++) {
            int c = pivot_cols[r];
            if (c <= p && temp[c]) temp ^= m[r];
        }
        return temp.none();
    };

    if (!reachable(rows.size() - 1)) {
        cout << "impossible";
        exit(0);
    }

    int l = -1, r = rows.size(), m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (reachable(m)) r = m;
        else l = m;
    }
    auto f = times[r];
    cout << fixed << setprecision(6) << (double) f.numer() / f.denom();
}
