#include <bits/stdc++.h>
using namespace std;

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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = numeric_limits<T>::max() / 4) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    friend auto operator*(const Matrix<T> &A, const Matrix<T> &B) {
        int r1 = A.r, c1 = A.c, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int k = 0; k < c1; k++)
                if (A[i][k] != numeric_limits<T>::max() / 4)
                    for (int j = 0; j < c2; j++) C[i][j] = min(C[i][j], A[i][k] + B[k][j]);
        return C;
    }

    friend auto operator*=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }
};

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n, n);
    for (int i = 0; i < n; i++) I[i][i] = 0;
    return I;
}

template <typename T, typename U>
Matrix<T> matpow(Matrix<T> A, U exponent) {
    int n = A.r;
    auto B = I<T>(n);

    while (exponent) {
        if (exponent & 1) B = A * B;
        A = A * A;
        exponent >>= 1;
    }
    return B;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    k = min(k, m);

    vector<Point<long long>> onions(n), polygon(m);
    for (auto &[x, y] : onions) cin >> x >> y;
    for (auto &[x, y] : polygon) cin >> x >> y;

    polygon.resize(2 * m);
    for (int i = 0; i < m; i++) polygon[i + m] = polygon[i];

    vector<vector<int>> count(m, vector<int>(m, 0));
    for (auto p : onions)
        for (int l = 0, r = 2; l < m; l++) {
            for (r = max(r, l + 2); r < l + m - 1 && cross(p, polygon[l], polygon[r]) < 0; r++);
            count[l][r - l]++;
        }

    Matrix<int> dp(m, m);
    for (int l = 0; l < m; l++) {
        for (int r = 1, pref = 0; r < m; r++) count[l][r] = pref += count[l][r];
        for (int r = l + 1; r < m; r++) dp[l][r] = count[l][r - l];
    }
    dp = matpow(dp, k - 1);

    int stolen = n;
    for (int i = 0; i < m; i++)
        for (int j = i + 1; j < m; j++)
            if (dp[i][j] < n) stolen = min(stolen, dp[i][j] + count[j][m - (j - i)]);
    cout << n - stolen;
}
