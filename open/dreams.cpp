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

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }
};

template <typename T>
vector<T> conjugate_gradient(const Matrix<T> &A, const vector<T> &b, T x0 = 0) {
    int n = b.size();

    vector<T> x(n, x0), r = b;
    if (x0 != (T) 0)
        for (int i = 0; i < n; i++) {
            T sum = 0;
            for (int j = 0; j < n; j++) sum += A[i][j];
            r[i] -= x0 * sum;
        }

    vector<T> p = r, Ap(n);
    T squared_norm = inner_product(r.begin(), r.end(), r.begin(), (T) 0), epsilon = numeric_limits<T>::epsilon();
    for (int _ = 0; _ < n && squared_norm > epsilon; _++) {
        T pAp = 0;
        for (int i = 0; i < n; i++) {
            T pi = p[i], sum = A[i][i] * pi;
            pAp += sum * pi;
            for (int j = 0; j < i; j++) {
                T a = A[i][j], pj = p[j];
                sum += a * pj;
                Ap[j] += a * pi;
                pAp += 2 * a * pi * pj;
            }
            Ap[i] = sum;
        }

        T alpha = squared_norm / pAp;
        for (int i = 0; i < n; i++) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }

        T temp = inner_product(r.begin(), r.end(), r.begin(), (T) 0);
        if (temp <= epsilon) return x;

        T beta = temp / squared_norm;
        for (int i = 0; i < n; i++) p[i] = r[i] + beta * p[i];
        squared_norm = temp;
    }

    return x;
}

int main() {
    ios::sync_with_stdio(false);
    Scanner scan;

    int n;
    scan >> n;

    vector<double> b(n), s(n);
    scan >> b >> s;

    Matrix<double> A(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++) {
            scan >> A[i][j];

            A[j][i] = A[i][j];
        }

    auto x = conjugate_gradient(A, b);
    auto m = inner_product(s.begin(), s.end(), x.begin(), 0.);
    for (int i = 0; i < n; i++) {
        auto fx = 0.;
        for (int j = 0; j < i; j++) {
            double fij;
            scan >> fij;

            fx += fij * x[j];
        }

        double fii;
        scan >> fii;

        m += 2 * x[i] * fx + fii * x[i] * x[i];
    }
    cout << fixed << setprecision(5) << m;
}