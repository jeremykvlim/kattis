#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    static inline array<int, 5> base{0, 1, 2, 3, 4};

    struct Monoid {
        array<int, 5> count;

        Monoid() : count{} {}

        auto & operator=(const char &c) {
            count[c - 'a'] = 1;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            for (int c = 0; c < 5; c++) count[c] += monoid.count[c];
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;
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
        if (lazy[i] != base) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = base;
        }
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    int kth(int k, int c) {
        return kth(1, k, c, 0, n);
    }

    int kth(int i, int k, int c, int l, int r) {
        if (l + 1 == r) return l;

        push(i);

        int m = midpoint(l, r), lc = ST[i << 1].count[c];
        if (k <= lc) return kth(i << 1, k, c, l, m);
        return kth(i << 1 | 1, k - lc, c, m, r);
    }

    void range_update(int l, int r, const array<int, 5> &v) {
        range_update(1, l, r, v, 0, n);
    }

    void range_update(int i, int ql, int qr, const array<int, 5> &v, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            apply(i, v);
            return;
        }

        push(i);

        int m = midpoint(l, r);
        range_update(i << 1, ql, qr, v, l, m);
        range_update(i << 1 | 1, ql, qr, v, m, r);

        pull(i);
    }

    Monoid range_query(int l, int r) {
        return range_query(1, l, r, 0, n);
    }

    Monoid range_query(int i, int ql, int qr, int l, int r) {
        if (qr <= l || r <= ql) return {};
        if (ql <= l && r <= qr) return ST[i];

        push(i);

        int m = midpoint(l, r);
        return range_query(i << 1, ql, qr, l, m) + range_query(i << 1 | 1, ql, qr, m, r);
    }

    string walk() {
        return walk(1, 0, n);
    }

    string walk(int i, int l, int r) {
        if (l + 1 == r) {
            for (int c = 0; c < 5; c++)
                if (ST[i].count[c]) return string(1, 'a' + c);
            return string(1, 'a');
        }

        push(i);

        int m = midpoint(l, r);
        return walk(i << 1, l, m) + walk(i << 1 | 1, m, r);
    }

    RURQSegmentTree(int n, const string &s) : n(n), ST(2 * n), lazy(n, base) {
        int m = bit_ceil(s.size());
        for (int i = 0; i < s.size(); i++) ST[(i + m) % n + n] = s[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string s;
    cin >> n >> m >> s;

    RURQSegmentTree st(n, s);
    while (m--) {
        int p;
        char ai, bi;
        cin >> p >> ai >> bi;

        auto v = st.base;
        int a = ai - 'a', b = bi - 'a';
        v[a] = b;
        st.range_update(0, st.kth(p, a) + 1, v);
    }
    cout << st.walk();
}
