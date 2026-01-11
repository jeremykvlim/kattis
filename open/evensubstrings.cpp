#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        array<int, 1 << 6> count;
        int mask;

        Monoid() : count{}, mask(0) {
            count.fill(0);
            count[0] = 1;
        }

        auto & operator=(const char &c) {
            count.fill(0);
            count[0] = count[mask = 1 << (c - 'a')] = 1;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            for (int m = 0; m < 64; m++) count[m] += monoid.count[m ^ mask];
            count[mask]--;
            mask ^= monoid.mask;
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

    void point_update(int i, const char &v) {
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

    PURQSegmentTree(int n, const string &s) : n(n), ST(2 * n) {
        for (int i = 0; i < s.size(); i++) ST[i + n] = s[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    PURQSegmentTree st(s.size(), s);
    while (q--) {
        int t;
        cin >> t;
        
        if (t == 1) {
            int l, r;
            cin >> l >> r;

            auto monoid = st.range_query(l - 1, r);
            auto count = 0LL;
            for (int mask = 0; mask < 1 << 6; mask++) count += ((long long) monoid.count[mask] * (monoid.count[mask] - 1)) / 2;
            cout << count << "\n";
        } else {
            int i;
            char x;
            cin >> i >> x;

            if (s[i - 1] == x) continue;
            s[i - 1] = x;
            st.point_update(i - 1, x);
        }
    }
}
