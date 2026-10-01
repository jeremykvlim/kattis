#include <bits/stdc++.h>
using namespace std;

struct Scanner {
    static constexpr int size = 1 << 20;
    array<char, size + 1> buf{};
    int pos = 0, len = 0;
    bool valid = true;

    inline bool reload() {
        pos = 0;
        len = fread(buf.data(), 1, size, stdin);
        buf[len] = 0;
        return len;
    }

    inline bool skip_space() {
        for (;;) {
            for (; buf[pos] == ' ' || buf[pos] == '\n' || buf[pos] == '\r' || buf[pos] == '\t'; pos++);
            if (pos < len) return true;
            if (!reload()) return false;
        }
    }

    template <typename T>
    inline bool read(T &v) {
        if (!skip_space()) return false;

        if constexpr (is_integral_v<T> && !is_same_v<T, char>) {
            bool neg = false;
            if (buf[pos] == '+' || buf[pos] == '-') {
                neg = buf[pos] == '-';
                if (++pos == len && !reload()) return false;
            }

            v = 0;
            for (;;) {
                for (; '0' <= buf[pos] && buf[pos] <= '9'; pos++) v = v * 10 + (buf[pos] - '0');
                if (pos < len || !reload()) break;
            }
            if (neg) v = -v;
            return true;
        } else if constexpr (is_floating_point_v<T>) {
            bool neg = false;
            if (buf[pos] == '+' || buf[pos] == '-') {
                neg = buf[pos] == '-';
                if (++pos == len && !reload()) return false;
            }

            v = 0;
            for (;;) {
                for (; '0' <= buf[pos] && buf[pos] <= '9'; pos++) v = v * 10 + (buf[pos] - '0');
                if (pos < len || !reload()) break;
            }
            if (buf[pos] == '.') {
                if (++pos == len) reload();
                T place = 1;
                for (;;) {
                    for (; '0' <= buf[pos] && buf[pos] <= '9'; pos++) {
                        place *= (T) 0.1;
                        v += (buf[pos] - '0') * place;
                    }
                    if (pos < len || !reload()) break;
                }
            }
            if (neg) v = -v;
            return true;
        } else if constexpr (is_same_v<T, char>) {
            v = buf[pos++];
            return true;
        } else if constexpr (is_same_v<T, string>) {
            v.clear();
            for (;;) {
                int prev = pos;
                for (; buf[pos] && buf[pos] != ' ' && buf[pos] != '\n' && buf[pos] != '\r' && buf[pos] != '\t'; pos++);
                v.append(buf.begin() + prev, buf.begin() + pos);
                if (pos < len || !reload()) break;
            }
            return true;
        }

        return false;
    }

    template <typename T>
    inline bool read(vector<T> &v) {
        for (auto &x : v)
            if (!read(x)) return false;
        return true;
    }

    template <typename T, typename U>
    inline bool read(pair<T, U> &p) {
        return read(p.first) && read(p.second);
    }

    template <typename... T>
    inline bool read(tuple<T...> &t) {
        return apply([&](auto &...x) {
            return (read(x) && ...);
        }, t);
    }

    template <typename... T>
    inline bool read(T &...x) requires (sizeof...(T) > 1) {
        return (read(x) && ...);
    }

    template <typename T>
    inline Scanner & operator>>(T &v) {
        if (valid) valid = read(v);
        return *this;
    }

    explicit operator bool() const {
        return valid;
    }
};

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

    auto begin() {
        return mat.begin();
    }

    auto end() {
        return mat.end();
    }

    auto begin() const {
        return mat.begin();
    }

    auto end() const {
        return mat.end();
    }
};

template <typename T>
bool freivalds(const Matrix<T> &A, const Matrix<T> &B, const Matrix<T> &C, int k = 1) {
    int n = A.r;
    if (A.c != n || B.r != n || B.c != n || C.r != n || C.c != n) return false;

    static mt19937_64 rng(random_device{}());
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
    Scanner scan;

    int n;
    while (scan >> n && n) {
        Matrix<long long> A(n), C(n);
        for (auto &row : A) scan >> row;
        for (auto &row : C) scan >> row;
        cout << (freivalds(A, A, C, 10) ? "YES\n" : "NO\n");
    }
}
