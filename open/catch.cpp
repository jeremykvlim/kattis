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
        return apply([&](auto &...x) { return (read(x) && ...); }, t);
    }

    template <typename... T>
    inline bool read(T &...x) requires (sizeof...(T) > 1) {
        return (read(x) && ...);
    }

    template <typename T>
    requires (!requires(Scanner &s, T &v) { operator>>(s, v); })
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

    int m, n;
    long long k;
    scan >> m >> n >> k;

    vector<tuple<int, int, double>> buses(m);
    vector<pair<long long, int>> times(2 * m);
    for (int i = 0; i < m; i++) {
        auto &[a, b, p] = buses[i];
        long long s, t;
        scan >> a >> b >> s >> t >> p;

        times[2 * i] = {s, i};
        times[2 * i + 1] = {t, ~i};
    }
    sort(times.begin(), times.end(), [&](auto p1, auto p2) { return p1.first != p2.first ? p1.first > p2.first : p1.second < p2.second; });

    vector<long long> prev(n, -1);
    vector<double> dp1(n, 0), dp2(n, 0), dp3(m, 0);
    dp1[1] = 1;
    for (auto [t, i] : times)
        if (i < 0) {
            auto &[a, b, p] = buses[~i];
            dp3[~i] = dp1[b];
        } else {
            auto &[a, b, p] = buses[i];
            if (prev[a] != t) {
                prev[a] = t;
                dp2[a] = dp1[a];
            }
            dp1[a] = max(dp1[a], dp2[a] + p * (dp3[i] - dp2[a]));
        }
    cout << fixed << setprecision(6) << dp1[0];
}
