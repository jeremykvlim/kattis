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

struct SegmentTree {
    static inline array<int, 5> identity;

    struct Segment {
        array<int, 5> count;

        Segment() {
            fill(count.begin(), count.end(), 0);
        }

        auto & operator=(const char &c) {
            count[c - 'a'] = 1;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            for (int c = 0; c < 5; c++) count[c] += seg.count[c];
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n;
    vector<Segment> ST;
    vector<array<int, 5>> lazy;

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, const array<int, 5> &v) {
        array<int, 5> temp{};
        for (int c = 0; c < 5; c++) temp[v[c]] += ST[i].count[c];
        ST[i].count = temp;
        if (i < n) {
            for (int c = 0; c < 5; c++) temp[c] = v[lazy[i][c]];
            lazy[i] = temp;
        }
    }

    void push(int i) {
        if (lazy[i] != identity) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = identity;
        }
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    int kth(int c, int k) {
        return kth(1, c, k, 0, n);
    }

    int kth(int i, int c, int k, int l, int r) {
        if (l + 1 == r) return l;

        push(i);

        int m = midpoint(l, r), lc = ST[i << 1].count[c];
        if (k <= lc) return kth(i << 1, c, k, l, m);
        return kth(i << 1 | 1, c, k - lc, m, r);
    }

    void modify(int l, int r, int a, int b) {
        auto v = identity;
        v[a] = b;
        modify(1, l, r, v, 0, n);
    }

    void modify(int i, int ql, int qr, const array<int, 5> &v, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            apply(i, v);
            return;
        }

        push(i);

        int m = midpoint(l, r);
        modify(i << 1, ql, qr, v, l, m);
        modify(i << 1 | 1, ql, qr, v, m, r);

        pull(i);
    }

    void query(string &s) {
        return query(1, s, 0, n);
    }

    void query(int i, string &s, int l, int r) {
        if (l + 1 == r) {
            for (int c = 0; c < 5; c++)
                if (ST[i].count[c]) {
                    s[l] = 'a' + c;
                    break;
                }
            return;
        }

        push(i);

        int m = midpoint(l, r);
        query(i << 1, s, l, m);
        query(i << 1 | 1, s, m, r);
    }

    SegmentTree(int n, const string &s) : n(n), ST(2 * n), lazy(n) {
        iota(identity.begin(), identity.end(), 0);
        fill(lazy.begin(), lazy.end(), identity);
        int m = bit_ceil(s.size());
        for (int i = 0; i < s.size(); i++) ST[(i + m) % n + n] = s[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    string s;
    read(n, m, s);

    SegmentTree st(n, s);
    while (m--) {
        int p;
        char ai, bi;
        read(p, ai, bi);

        int a = ai - 'a', b = bi - 'a';
        st.modify(0, st.kth(a, p) + 1, a, b);
    }

    string chain(n, 'a');
    st.query(chain);
    cout << chain;
}
