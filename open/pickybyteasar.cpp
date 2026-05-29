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

    int n, h, m;
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

    void push_down(int l, int r) {
        for (int b = h; b; b--) {
            if (((l >> b) << b) != l) push(l >> b);
            if (((r >> b) << b) != r) push((r - 1) >> b);
        }
    }

    void pull_up(int l, int r) {
        for (int b = 1; b <= h; b++) {
            if (((l >> b) << b) != l) pull(l >> b);
            if (((r >> b) << b) != r) pull((r - 1) >> b);
        }
    }

    void range_update(int l, int r, const array<int, 5> &v) {
        l += n;
        r += n;
        push_down(l, r);

        int temp_l = l, temp_r = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, v);
            if (r & 1) apply(--r, v);
        }

        pull_up(temp_l, temp_r);
    }

    Monoid range_query(int l, int r) {
        l += n;
        r += n;
        push_down(l, r);

        Monoid ml, mr;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    int kth(int k, int c) {
        int i = 1;

        for (;;) {
            if (i >= n) return i - n;

            push(i);

            int left = i << 1;
            if (k <= ST[left].count[c]) {
                i = left;
            } else {
                k -= ST[left].count[c];
                i = left | 1;
            }
        }
    }

    string walk() {
        for (int i = 1; i < n; i++) push(i);

        string s;
        for (int i = 0; i < m; i++)
            for (int c = 0; c < 5; c++)
                if (ST[i + n].count[c]) {
                    s += char('a' + c);
                    break;
                }

        return s;
    }

    RURQSegmentTree(int n, const string &s) : n(n), h(__lg(n)), ST(2 * n), lazy(n, base), m(s.size()) {
        for (int i = 0; i < s.size(); i++) ST[i + n] = s[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string s;
    cin >> n >> m >> s;

    RURQSegmentTree st(bit_ceil((unsigned) n), s);
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