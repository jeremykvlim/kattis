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

int main() {
    ios::sync_with_stdio(false);

    int n;
    read(n);

    vector<double> pref_x(n + 1, 0), pref_y(n + 1, 0), pref_xx(n + 1, 0), pref_xy(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        double x, y;
        read(x, y);

        pref_x[i] = pref_x[i - 1] + x;
        pref_y[i] = pref_y[i - 1] + y;
        pref_xx[i] = pref_xx[i - 1] + x * x;
        pref_xy[i] = pref_xy[i - 1] + x * y;
    }

    int m;
    read(m);

    while (m--) {
        int l, r;
        double lambda, x;
        read(l, r, lambda, x);

        auto sx = pref_x[r] - pref_x[l - 1], sy = pref_y[r] - pref_y[l - 1], sxx = pref_xx[r] - pref_xx[l - 1], sxy = pref_xy[r] - pref_xy[l - 1],
             a = sx / (sxx + lambda), b = (sxy * a - sy) / (sx * a - (r - l + 1) - lambda), k = (sxy - b * sx) / (sxx + lambda);
        cout << fixed << setprecision(6) << k * x + b << "\n";
    }
}
