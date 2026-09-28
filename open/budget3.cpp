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
                for (; buf[pos] && buf[pos] != ' ' && buf[pos] != '\n' && buf[pos] != '\r' && buf[pos] != '\t'; pos++)
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

int main() {
    ios::sync_with_stdio(false);
    Scanner scan;

    int n;
    scan >> n;

    vector<double> pref_x(n + 1, 0), pref_y(n + 1, 0), pref_xx(n + 1, 0), pref_xy(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        double x, y;
        scan >> x >> y;

        pref_x[i] = pref_x[i - 1] + x;
        pref_y[i] = pref_y[i - 1] + y;
        pref_xx[i] = pref_xx[i - 1] + x * x;
        pref_xy[i] = pref_xy[i - 1] + x * y;
    }

    int m;
    scan >> m;

    while (m--) {
        int l, r;
        double lambda, x;
        scan >> l >> r >> lambda >> x;

        auto sx = pref_x[r] - pref_x[l - 1], sy = pref_y[r] - pref_y[l - 1], sxx = pref_xx[r] - pref_xx[l - 1], sxy = pref_xy[r] - pref_xy[l - 1],
             a = sx / (sxx + lambda), b = (sxy * a - sy) / (sx * a - (r - l + 1) - lambda), k = (sxy - b * sx) / (sxx + lambda);
        cout << fixed << setprecision(6) << k * x + b << "\n";
    }
}
