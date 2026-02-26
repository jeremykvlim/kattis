#include <bits/stdc++.h>
using namespace std;

inline char readchar() {
    static const int size = 1 << 20;
    static array<char, size> buf;
    static int pos = 0, len = 0;
    if (pos >= len) {
        pos = 0;
        len = fread(buf.data(), 1, size, stdin);
        if (!len) return EOF;
    }
    return buf[pos++];
}

template <typename T>
inline bool read(T &v) {
    char c;
    do {
        c = readchar();
        if (c == EOF) return false;
    } while (c == ' ' || c == '\n' || c == '\r' || c == '\t');

    if constexpr (is_integral_v<T> && !is_same_v<T, char>) {
        bool neg = false;
        if (c == '+' || c == '-') {
            neg = (c == '-');
            c = readchar();
        }

        v = 0;
        for (; '0' <= c && c <= '9'; c = readchar()) v = v * 10 + (c - '0');
        if (neg) v = -v;
        return true;
    } else if constexpr (is_floating_point_v<T>) {
        bool neg = false;
        if (c == '+' || c == '-') {
            neg = (c == '-');
            c = readchar();
        }

        v = 0;
        for (; '0' <= c && c <= '9'; c = readchar()) v = v * 10 + (c - '0');
        if (c == '.') {
            T place = 1;
            for (c = readchar(); '0' <= c && c <= '9'; c = readchar()) {
                place *= 0.1;
                v += (c - '0') * place;
            }
        }
        if (neg) v = -v;
        return true;
    } else if constexpr (is_same_v<T, char>) {
        v = c;
        return true;
    } else if constexpr (is_same_v<T, string>) {
        v.clear();
        do {
            v += c;
            c = readchar();
        } while (c != EOF && c != ' ' && c != '\n' && c != '\r' && c != '\t');
        return true;
    }

    return false;
}

template <typename... T>
inline bool read(T &... xs) requires (sizeof...(T) > 1) {
    return (read(xs) && ...);
}

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    friend auto operator*(const Matrix<T> &A, const vector<T> &v) {
        int n = A.r, m = v.size();

        vector<T> u(n, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) u[i] += A[i][j] * v[j];

        return u;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }
};

template <typename T>
bool freivalds(const Matrix<T> &A, const Matrix<T> &B, const Matrix<T> &C, int k = 1) {
    int n = A.r;
    if (A.c != n || B.r != n || B.c != n || C.r != n || C.c != n) return false;

    mt19937_64 rng(random_device{}());
    vector<T> r(n), br(n), abr(n), cr(n);
    while (k--) {
        for (T &ri : r) ri = rng() & 1;

        br = B * r;
        abr = A * br;
        cr = C * r;

        for (int i = 0; i < n; i++)
            if (abr[i] != cr[i]) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    while (read(n) && n) {
        Matrix<long long> A(n), C(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) read(A[i][j]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) read(C[i][j]);

        cout << (freivalds(A, A, C, 10) ? "YES\n" : "NO\n");
    }
}
