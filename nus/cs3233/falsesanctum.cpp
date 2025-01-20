#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long density;
        char c_l, c_r;
        int len_l, len_r, len;

        Segment(bool empty = false) : density(0), c_l('#'), c_r('#'), len_l(1), len_r(1), len(empty ? 0 : 1) {}

        auto & operator=(const char &c) {
            density = len_l = len_r = 1;
            c_l = c_r = c;
            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            if (!sl.len) return sr;
            if (!sr.len) return sl;

            Segment seg;
            seg.density = sl.density + sr.density;
            seg.c_l = sl.c_l;
            seg.c_r = sr.c_r;
            seg.len_l = sl.len_l;
            seg.len_r = sr.len_r;
            seg.len = sl.len + sr.len;

            if (sl.len_l == sl.len && sl.c_l == sr.c_l) seg.len_l += sr.len_l;
            if (sr.len_r == sr.len && sr.c_r == sl.c_r) seg.len_r += sl.len_r;
            if (sl.c_r == sr.c_l) {
                seg.density += (long long) (sl.len_r + sr.len_l) * (sl.len_r + sr.len_l);
                seg.density -= (long long) sl.len_r * sl.len_r + (long long) sr.len_l * sr.len_l;
            }
            return seg;
        }
    };

    string str;
    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void modify(const int &pos) {
        modify(1, pos, 0, n);
    }

    void modify(int i, const int &pos, int l, int r) {
        if (l + 1 == r) {
            ST[i] = str[l];
            return;
        }

        int m = midpoint(l, r);
        if (pos < m) modify(i << 1, pos, l, m);
        else modify(i << 1 | 1, pos, m, r);

        pull(i);
    }

    Segment range_query(int l, int r) {
        return range_query(1, l, r, 0, n);
    }

    Segment range_query(int i, int ql, int qr, int l, int r) {
        if (qr <= l || r <= ql) return {true};
        if (ql <= l && r <= qr) return ST[i];

        int m = midpoint(l, r);
        return range_query(i << 1, ql, qr, l, m) + range_query(i << 1 | 1, ql, qr, m, r);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, string str) : n(n), ST(2 * n), str(str) {
        int m = bit_ceil((unsigned) n);
        for (int i = 0; i < str.size(); i++) ST[(i + m) % n + n] = str[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    string s;
    cin >> n >> Q >> s;

    SegmentTree st(n, s);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int i;
            char c;
            cin >> i >> c;

            st.str[i - 1] = c;
            st.modify(i - 1);
        } else if (q == 2) {
            int l, r;
            cin >> l >> r;

            auto seg = st.range_query(l - 1, r);
            cout << seg.density << "\n";
        }
    }
}
