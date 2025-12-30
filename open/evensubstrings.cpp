#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Segment {
        array<int, 1 << 6> count;
        int mask;

        Segment() : count{}, mask(0) {
            count.fill(0);
            count[0] = 1;
        }

        auto & operator=(const char &c) {
            count.fill(0);
            count[0] = count[mask = 1 << (c - 'a')] = 1;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            for (int m = 0; m < 64; m++) count[m] += seg.count[m ^ mask];
            count[mask]--;
            mask ^= seg.mask;
            return *this;
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

            auto seg = st.range_query(l - 1, r);
            auto count = 0LL;
            for (int mask = 0; mask < 1 << 6; mask++) count += ((long long) seg.count[mask] * (seg.count[mask] - 1)) / 2;
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
