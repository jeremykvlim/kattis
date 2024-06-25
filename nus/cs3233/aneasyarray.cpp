#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        int min1, min2, max1, max2;

        Segment(): min1(INT_MAX), min2(INT_MAX), max1(INT_MIN), max2(INT_MIN) {}

        auto & operator=(int v) {
            min1 = max1 = v;
            return *this;
        }

        auto operator+=(Segment seg) {
            min2 = min({max(min1, seg.min1), min2, seg.min2});
            min1 = min(min1, seg.min1);
            max2 = max({min(max1, seg.max1), max2, seg.max2});
            max1 = max(max1, seg.max1);

            return *this;
        }

        friend auto operator+(Segment sl, Segment sr) {
            return sl += sr;
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

    void assign(int i, int v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto query(int l, int r) {
        Segment seg;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) seg += ST[l++];
            if (r & 1) seg += ST[--r];
        }

        return seg;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, vector<int> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    SegmentTree st(n, a);
    while (q--) {
        int l, r;
        cin >> l >> r;

        auto v = (long long) a[l - 1] * a[r - 1];
        auto s = st.query(l, r - 1);
        cout << max({v * s.min1 * s.min2, v * s.min1 * s.max1, v * s.max1 * s.max2}) << "\n";
    }
}
