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

struct KineticTournament {
    static inline vector<long long> A, S;
    static inline vector<bool> active;
    static inline int time, C;

    static long long cost(int l, int r) {
        return S[l] + (A[r] - A[l + 1]) * (r - l) + C;
    }

    static long long eval(int i) {
        if (i < 0 || !active[i]) return -1e18;
        return -cost(i, time);
    }

    static int overtake(int i, int j) {
        if (j < 0 || !active[j]) return 1e9;
        if (i < 0 || !active[i]) return 0;
        if (cost(i, time) > cost(j, time)) return 0;
        int l = time, r = A.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (cost(i, m) > cost(j, m)) r = m;
            else l = m;
        }
        return r;
    }

    struct Segment {
        int index, t;

        Segment(int i = -1, int t = 1e9) : index(i), t(t) {}

        friend Segment operator+(const Segment &sl, const Segment &sr) {
            int l = sl.index, r = sr.index;
            if (eval(l) < eval(r)) swap(l, r);
            return {l, min({sl.t, sr.t, overtake(l, r)})};
        }
    };

    int n;
    vector<Segment> KT;

    KineticTournament(int n, const vector<long long> &a, int c) : n(n), KT(2 * n) {
        time = 0;
        active = vector<bool>(n, false);
        A = a;
        S = vector<long long>(n, 0);
        C = c;
    }

    void pull(int i) {
        KT[i] = KT[i << 1] + KT[i << 1 | 1];
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void modify(const int &pos) {
        modify(1, pos, 0, n);
    }

    void modify(int i, int pos, int l, int r) {
        if ((pos < l || pos >= r) && KT[i].t > time) return;
        if (l + 1 == r) {
            if (active[l]) KT[i] = l;
            return;
        }

        int m = midpoint(l, r);
        modify(i << 1, pos, l, m);
        modify(i << 1 | 1, pos, m, r);
        pull(i);
    }

    long long query(int t) {
        time = t;
        modify(-1);
        return eval(KT[1].index);
    }

    void activate(int i) {
        active[i] = true;
        modify(i);
    }

    void toggle(int i) {
        active[i] = !active[i];
        modify(i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    read(n, c);

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) read(a[i]);

    KineticTournament kt(n + 1, a, c);
    kt.activate(0);
    for (int i = 1; i <= n; i++) {
        int k;
        read(k);

        kt.S[i] = -kt.query(i);
        if (i == n) break;
        kt.activate(i);
        kt.toggle(((kt.S[i] + k) % i) + 1);
    }
    cout << kt.S[n];
}
