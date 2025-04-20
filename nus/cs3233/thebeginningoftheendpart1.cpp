#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long even, odd;
        int count;

        Segment() : even(0), odd(0), count(0) {}

        auto & operator+=(const int &v) {
            even = (long long) count / 2 * v;
            odd = (long long) (count + 1) / 2 * v;
            return *this;
        }

        auto & operator=(const Segment &s) {
            even = s.even;
            odd = s.odd;
            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            seg.even = sl.even + (sl.count & 1 ? sr.odd : sr.even);
            seg.odd = sl.odd + (sl.count & 1 ? sr.even : sr.odd);
            return seg;
        }
    };

    int n;
    vector<Segment> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, const int &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        if (lazy[i]) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = 0;
        }
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void modify(const int &pos, const int &v) {
        modify(1, pos, v, 0, n);
    }

    void modify(int i, const int &pos, const int &v, int l, int r) {
        ST[i].count++;
        if (l + 1 == r) {
            apply(i, v);
            return;
        }

        push(i);

        int m = midpoint(l, r);
        if (pos < m) modify(i << 1, pos, v, l, m);
        else modify(i << 1 | 1, pos, v, m, r);

        pull(i);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n) : n(n), ST(2 * n), lazy(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    SegmentTree st(4e5);
    auto g = 0LL;
    for (int i = 0; i < n; i++) {
        int gi, bi;
        cin >> gi >> bi;

        g += gi;
        st.modify(gi + bi, gi + bi);

        if (g > (i & 1 ? st[1].odd : st[1].even)) cout << "G\n" << flush;
        else if (g < (i & 1 ? st[1].odd : st[1].even)) cout << "B\n" << flush;
        else cout << "S\n" << flush;
    }
}
