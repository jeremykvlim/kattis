#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        array<int, 10> count;
        array<int, 45> both;
        int len;
        long long sum;

        Monoid() : count{}, both{}, len(0), sum(0) {}

        auto & operator=(const int &v) {
            len = 1;
            sum = (long long) v * v;
            for (int i = 0; i < 10; i++) count[i] = (v >> i) & 1;
            for (int i = 0, k = 0; i < 10; i++)
                for (int j = i + 1; j < 10; j++, k++) both[k] = count[i] && count[j];
            return *this;
        }

        auto & operator+=(const int &v) {
            if (!v) return *this;

            auto temp = count;
            for (int i = 0; i < 10; i++)
                if ((v >> i) & 1) count[i] = len - count[i];
            
            sum = 0;
            for (int i = 0; i < 10; i++) sum += (long long) count[i] << (2 * i);

            for (int i = 0, k = 0; i < 10; i++) {
                bool bi = (v >> i) & 1;
                for (int j = i + 1; j < 10; j++, k++) {
                    bool bj = (v >> j) & 1;
                    if (bi && bj) both[k] += len - temp[i] - temp[j];
                    else if (bi) both[k] = temp[j] - both[k];
                    else if (bj) both[k] = temp[i] - both[k];
                    sum += (long long) both[k] << (i + j + 1);
                }
            }
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            for (int i = 0; i < 10; i++) count[i] += monoid.count[i];
            for (int i = 0; i < 45; i++) both[i] += monoid.both[i];
            len += monoid.len;
            sum += monoid.sum;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const int &v) {
        ST[i] += v;
        if (i < n) lazy[i] ^= v;
    }

    void push(int i) {
        for (int k = h; k; k--) {
            int j = i >> k;
            if (lazy[j]) {
                apply(j << 1, lazy[j]);
                apply(j << 1 | 1, lazy[j]);
                lazy[j] = 0;
            }
        }
    }

    void range_update(int l, int r, const int &v) {
        push(l + n);
        push(r + n - 1);
        bool cl = false, cr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (cl) pull(l - 1);
            if (cr) pull(r);
            if (l & 1) {
                cl = true;
                apply(l++, v);
            }
            if (r & 1) {
                cr = true;
                apply(--r, v);
            }
        }

        for (l--; r; l >>= 1, r >>= 1) {
            if (cl) pull(l);
            if (cr && (!cl || l < r)) pull(r);
        }
    }

    Monoid range_query(int l, int r) {
        push(l + n);
        push(r + n - 1);
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

    RURQSegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
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

    RURQSegmentTree st(bit_ceil((unsigned) n), a);
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;

        if (type == 1) {
            int x;
            cin >> x;

            st.range_update(l - 1, r, x);
        } else cout << st.range_query(l - 1, r).sum << "\n";
    }
}