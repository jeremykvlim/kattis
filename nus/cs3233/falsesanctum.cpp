#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
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

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const char &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Segment range_query(int l, int r) {
        Segment sl, sr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = sl + ST[l++];
            if (r & 1) sr = ST[--r] + sr;
        }

        return sl + sr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n, const string &s) : n(n), ST(2 * n) {
        for (int i = 0; i < s.size(); i++) ST[i + n] = s[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    string s;
    cin >> n >> Q >> s;

    PURQSegmentTree st(n, s);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int i;
            char c;
            cin >> i >> c;

            st.point_update(i - 1, c);
        } else if (q == 2) {
            int l, r;
            cin >> l >> r;
            cout << st.range_query(l - 1, r).density << "\n";
        }
    }
}