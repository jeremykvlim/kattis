#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    static inline array<int, 55> delta;
    static inline vector<vector<int>> indices;

    struct Monoid {
        array<int, 55> sgn;
        long long sum;

        Monoid() : sgn{}, sum(0) {}

        auto & operator=(const int &v) {
            sum = v * v;
            array<int, 10> temp;
            for (int b = 0; b < 10; b++) sgn[b] = temp[b] = ((v >> b) & 1) ? -1 : 1;

            for (int i = 10, b1 = 0; b1 < 10; b1++)
                for (int b2 = b1 + 1; b2 < 10; b2++) sgn[i++] = temp[b1] * temp[b2];

            return *this;
        }

        auto & operator+=(const int &v) {
            for (int j : indices[v]) {
                sum += sgn[j] * delta[j];
                sgn[j] *= -1;
            }

            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            for (int i = 0; i < 55; i++) sgn[i] += monoid.sgn[i];
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

    RURQSegmentTree(int n, const vector<int> &a, const auto &d, const auto &ids) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
        delta = d;
        indices = ids;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    array<int, 55> delta;
    vector<vector<int>> indices(1024);
    for (int b = 0; b < 10; b++) delta[b] = ((1 << 10) - 1) * (1 << b);
    for (int i = 10, b1 = 0; b1 < 10; b1++)
        for (int b2 = b1 + 1; b2 < 10; b2++) delta[i++] = -(1 << b1) * (1 << b2);

    for (int mask = 0; mask < 1024; mask++) {
        for (int b = 0; b < 10; b++)
            if ((mask >> b) & 1) indices[mask].emplace_back(b);

        for (int i = 10, b1 = 0; b1 < 10; b1++)
            for (int b2 = b1 + 1; b2 < 10; b2++) {
                if (((mask >> b1) & 1) != ((mask >> b2) & 1)) indices[mask].emplace_back(i);
                i++;
            }
    }

    RURQSegmentTree st(bit_ceil((unsigned) n), a, delta, indices);
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
