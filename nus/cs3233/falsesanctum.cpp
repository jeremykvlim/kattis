#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        long long density;
        char c_l, c_r;
        int len_l, len_r, len;

        Monoid(bool empty = false) : density(0), c_l('#'), c_r('#'), len_l(1), len_r(1), len(empty ? 0 : 1) {}

        auto & operator=(const char &c) {
            density = len_l = len_r = 1;
            c_l = c_r = c;
            return *this;
        }

        friend auto operator+(const Monoid &ml, const Monoid &mr) {
            if (!ml.len) return mr;
            if (!mr.len) return ml;

            Monoid monoid;
            monoid.density = ml.density + mr.density;
            monoid.c_l = ml.c_l;
            monoid.c_r = mr.c_r;
            monoid.len_l = ml.len_l;
            monoid.len_r = mr.len_r;
            monoid.len = ml.len + mr.len;

            if (ml.len_l == ml.len && ml.c_l == mr.c_l) monoid.len_l += mr.len_l;
            if (mr.len_r == mr.len && mr.c_r == ml.c_r) monoid.len_r += ml.len_r;
            if (ml.c_r == mr.c_l) {
                monoid.density += (long long) (ml.len_r + mr.len_l) * (ml.len_r + mr.len_l);
                monoid.density -= (long long) ml.len_r * ml.len_r + (long long) mr.len_l * mr.len_l;
            }
            return monoid;
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

    int n, Q;
    string s;
    cin >> n >> Q >> s;

    PURQSegmentTree st(n, s);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int i;
            char c;
            cin >> i >> c;

            st.point_update(i - 1, c);
        } else if (q == 2) {
            int l, r;
            cin >> l >> r;
            cout << st.range_query(l - 1, r).density << "\n";
        }
    }
}