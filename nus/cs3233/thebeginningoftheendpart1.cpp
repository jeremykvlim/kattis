#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        long long even, odd;
        int count;

        Monoid() : even(0), odd(0), count(0) {}

        auto & operator+=(const int &v) {
            count++;
            even = (long long) (count / 2) * v;
            odd = (long long) (count + 1) / 2 * v;
            return *this;
        }

        auto & operator=(const int &v) {
            return *this += v;
        }

        friend auto operator+(const Monoid &ml, const Monoid &mr) {
            Monoid monoid;
            monoid.count = ml.count + mr.count;
            monoid.even = ml.even + (ml.count & 1 ? mr.odd : mr.even);
            monoid.odd = ml.odd + (ml.count & 1 ? mr.even : mr.odd);
            return monoid;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
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