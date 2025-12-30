#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Segment {
        long long even, odd;
        int count;

        Segment() : even(0), odd(0), count(0) {}

        auto & operator+=(const int &v) {
            count++;
            even = (long long) (count / 2) * v;
            odd = (long long) (count + 1) / 2 * v;
            return *this;
        }

        auto & operator=(const int &v) {
            return *this += v;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            seg.count = sl.count + sr.count;
            seg.even = sl.even + (sl.count & 1 ? sr.odd : sr.even);
            seg.odd = sl.odd + (sl.count & 1 ? sr.even : sr.odd);
            return seg;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void point_update(int i, const int &v) {
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

    PURQSegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    PURQSegmentTree st(bit_ceil((unsigned) 4e5));
    auto g = 0LL;
    for (int i = 0; i < n; i++) {
        int gi, bi;
        cin >> gi >> bi;

        g += gi;
        st.point_update(4e5 - gi - bi, gi + bi);
        cout << (g > st[1].even ? 'G' : (g < st[1].even ? 'B' : 'S')) << "\n" << flush;
    }
}