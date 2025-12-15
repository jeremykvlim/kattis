#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    int upper_bound(T k) {
        int i = 0;
        for (int m = bit_ceil(BIT.size()); m; m >>= 1)
            if (i + m < BIT.size() && BIT[i + m] <= k) {
                i += m;
                k -= BIT[i];
            }
        return i + 1;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

struct SegmentTree {
    struct Segment {
        long long value, sum, pref, suff;

        Segment() : value(0), sum(0), pref(0), suff(0) {}

        auto & operator=(const long long &v) {
            value = sum = pref = suff = v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            Segment s;
            s.value = max(max(value, seg.value), suff + seg.pref);
            s.sum = sum + seg.sum;
            s.pref = max(pref, sum + seg.pref);
            s.suff = max(seg.suff, seg.sum + suff);
            return s;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
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

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void modify(const int &pos, const long long &v) {
        modify(1, pos, v, 0, n);
    }

    void modify(int i, const int &pos, const long long &v, int l, int r) {
        if (l + 1 == r) {
            ST[i] = v;
            return;
        }

        int m = midpoint(l, r);
        if (pos < m) modify(i << 1, pos, v, l, m);
        else modify(i << 1 | 1, pos, v, m, r);

        pull(i);
    }

    Segment range_query(int l, int r) {
        return range_query(1, l, r, 0, n);
    }

    Segment range_query(int i, int ql, int qr, int l, int r) {
        if (qr <= l || r <= ql) return {};
        if (ql <= l && r <= qr) return ST[i];

        int m = midpoint(l, r);
        return range_query(i << 1, ql, qr, l, m) + range_query(i << 1 | 1, ql, qr, m, r);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<long long> &a) : n(n), ST(2 * n) {
        int m = bit_ceil((unsigned) n);
        for (int i = 0; i < a.size(); i++) ST[(i + m) % n + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<long long> h(n + 1);
    for (int i = 0; i <= n; i++) cin >> h[i];

    auto fun = [&](int i) {
        auto d = h[i] - h[i + 1];
        return h[i] >= h[i + 1] ? d * d : -d * d;
    };
    vector<long long> f(n);
    for (int i = 0; i < n; i++) f[i] = fun(i);

    FenwickTree<long long> fw(n + 1);
    for (int i = 0; i < n; i++) fw.update(i + 1, h[i] + h[i + 1]);

    SegmentTree st(n, f);
    while (q--) {
        int t;
        cin >> t;

        if (!t) {
            int i;
            long long v;
            cin >> i >> v;

            auto d = v - h[i];
            h[i] = v;
            if (i) {
                st.modify(i - 1, f[i - 1] = fun(i - 1));
                fw.update(i, d);
            }
            if (i < n) {
                st.modify(i, f[i] = fun(i));
                fw.update(i + 1, d);
            }
        } else {
            int x;
            long long k;
            cin >> x >> k;
            cout << st.range_query(0, fw.upper_bound(fw.pref_sum(x) + k) - 1).value << "\n";
        }
    }
}
