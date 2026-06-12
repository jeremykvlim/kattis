#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    static inline int k;

    struct Monoid {
        int len, subarray_len;
        long long mask;
        vector<pair<long long, int>> pref, suff;

        Monoid() : len(0), subarray_len(1e9), mask(0) {}

        auto & operator=(const int &v) {
            mask = 1LL << (v - 1);
            len = 1;
            subarray_len = k == 1 ? 1 : 1e9;
            pref = suff = {{mask, 1}};
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (!len) return *this = monoid;
            if (!monoid.len) return *this;

            Monoid m;
            m.len = len + monoid.len;
            m.mask = mask | monoid.mask;
            m.subarray_len = min(subarray_len, monoid.subarray_len);
            m.pref = pref;
            m.suff = monoid.suff;

            for (auto [p_mask, p_len] : monoid.pref) {
                auto merged = mask | p_mask;
                if (merged != m.pref.back().first) m.pref.emplace_back(merged, len + p_len);
            }

            int i = monoid.pref.size() - 1;
            for (auto [s_mask, s_len] : suff) {
                auto merged = monoid.mask | s_mask;
                if (merged != m.suff.back().first) m.suff.emplace_back(merged, monoid.len + s_len);
                if (merged == (1LL << k) - 1) {
                    for (; i && (s_mask | monoid.pref[i - 1].first) == (1LL << k) - 1; i--);
                    m.subarray_len = min(m.subarray_len, s_len + monoid.pref[i].second);
                }
            }

            return *this = m;
        }

        friend Monoid operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n, int K, const vector<int> &a) : n(n), ST(2 * n) {
        k = K;
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    cin >> n >> k >> m;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    PURQSegmentTree st(bit_ceil((unsigned) n), k, a);
    while (m--) {
        int q;
        cin >> q;

        if (q == 1) {
            int p, v;
            cin >> p >> v;

            if (a[p - 1] != v) st.point_update(p - 1, a[p - 1] = v);
        } else cout << (st[1].subarray_len == 1e9 ? -1 : st[1].subarray_len) << "\n";
    }
}
