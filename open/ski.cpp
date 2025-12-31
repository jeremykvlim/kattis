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

struct PURQSegmentTree {
    struct Segment {
        long long value, sum, pref, suff;

        Segment() : value(0), sum(0), pref(0), suff(0) {}

        auto & operator=(const long long &v) {
            value = sum = pref = suff = v;
            return *this;
        }

        auto & operator+=(const Segment &seg) {
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

    void point_update(int i, const long long &v) {
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

    PURQSegmentTree(int n, const vector<long long> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
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

    PURQSegmentTree st(n, f);
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
                st.point_update(i - 1, f[i - 1] = fun(i - 1));
                fw.update(i, d);
            }
            if (i < n) {
                st.point_update(i, f[i] = fun(i));
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