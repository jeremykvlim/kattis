#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        int min1, min2, max1, max2;

        Monoid() : min1(INT_MAX), min2(INT_MAX), max1(INT_MIN), max2(INT_MIN) {}

        auto & operator=(const int &v) {
            min1 = max1 = v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            min2 = min({max(min1, monoid.min1), min2, monoid.min2});
            min1 = min(min1, monoid.min1);
            max2 = max({min(max1, monoid.max1), max2, monoid.max2});
            max1 = max(max1, monoid.max1);

            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
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

    PURQSegmentTree(int n, const vector<int> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
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

    PURQSegmentTree st(n, a);
    while (q--) {
        int l, r;
        cin >> l >> r;

        auto v = (long long) a[l - 1] * a[r - 1];
        auto s = st.range_query(l, r - 1);
        cout << max({v * s.min1 * s.min2, v * s.min1 * s.max1, v * s.max1 * s.max2}) << "\n";
    }
}
