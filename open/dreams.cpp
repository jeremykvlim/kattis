#include <bits/stdc++.h>
using namespace std;

inline char readchar() {
    static const int size = 1 << 20;
    static char buf[size];
    static int pos = 0, len = 0;

    if (pos >= len) {
        pos = 0;
        len = fread(buf, 1, size, stdin);
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
vector<T> conjugate_gradient(const vector<vector<T>> &A, const vector<T> &b) {
    int n = b.size();

    vector<T> p = b, r = b, Ap(n), x(n, 0);
    T squared_norm = inner_product(r.begin(), r.end(), r.begin(), (T) 0);
    for (int _ = 0; _ < n; _++) {
        for (int i = 0; i < n; i++) {
            Ap[i] = 0;
            for (int j = 0; j < n; j++) Ap[i] += A[i][j] * p[j];
        }

        T pAp = 0;
        for (int i = 0; i < n; i++) pAp += p[i] * Ap[i];
        if (fabs(pAp) < 1e-10) break;

        T alpha = squared_norm / pAp;
        for (int i = 0; i < n; i++) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }

        T temp = inner_product(r.begin(), r.end(), r.begin(), (T) 0);
        if (sqrt(temp) < 1e-5) return x;

        T beta = temp / squared_norm;
        for (int i = 0; i < n; i++) p[i] = r[i] + beta * p[i];
        squared_norm = temp;
    }

    return x;
}

int main() {
    ios::sync_with_stdio(false);
    
    int n;
    read(n);

    vector<double> b(n), s(n);
    for (auto &bi : b) read(bi);
    for (auto &si : s) read(si);

    vector<vector<double>> A(n, vector<double>(n)), F(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++) {
            read(A[i][j]);

            A[j][i] = A[i][j];
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++) {
            read(F[i][j]);

            F[j][i] = F[i][j];
        }

    auto x = conjugate_gradient(A, b);
    vector<double> Fx(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) Fx[i] += F[i][j] * x[j];

    double m = 0;
    for (int i = 0; i < n; i++) m += x[i] * Fx[i] + s[i] * x[i];
    cout << fixed << setprecision(5) << m;
}
