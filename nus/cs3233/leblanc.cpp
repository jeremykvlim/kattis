#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        long long value;
        bool reduce, valid;

        Monoid() : value(0), reduce(false), valid(false) {}

        auto & operator+=(const long long &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (!monoid.valid) return *this;
            if (!valid) return *this = monoid;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<long long> lazy1;
    vector<bool> lazy2;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, const long long &v) {
        ST[i] += v;
        if (i < n) lazy1[i] += v;
    }

    void apply(int i) {
        ST[i].value = 0;
        ST[i].reduce = ST[i].valid = true;
        if (i < n) {
            lazy2[i] = true;
            lazy1[i] = 0;
        }
    }

    void push(int i) {
        for (int k = h; k; k--) {
            int j = i >> k;
            if (lazy2[j]) {
                apply(j << 1);
                apply(j << 1 | 1);
                lazy2[j] = false;
            }

            if (lazy1[j]) {
                apply(j << 1, lazy1[j]);
                apply(j << 1 | 1, lazy1[j]);
                lazy1[j] = 0;
            }
        }
    }

    void range_update(int l, int r) {
        push(l + n);
        push(r + n - 1);
        bool cl = false, cr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (cl) pull(l - 1);
            if (cr) pull(r);
            if (l & 1) {
                cl = true;
                apply(l++);
            }
            if (r & 1) {
                cr = true;
                apply(--r);
            }
        }

        for (l--; r; l >>= 1, r >>= 1) {
            if (cl) pull(l);
            if (cr && (!cl || l < r)) pull(r);
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

    RURQSegmentTree(int n) : n(n), h(__lg(n)), ST(2 * n), lazy1(n, 0), lazy2(n, false) {}
};

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

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    long long p;
    cin >> n >> q >> p;

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    FenwickTree<long long> fw(n + 2);
    RURQSegmentTree st(bit_ceil((unsigned) n));
    while (q--) {
        int op;
        cin >> op;

        if (op == 1) {
            int l, r, x;
            cin >> l >> r >> x;

            fw.update(l, x);
            fw.update(r + 1, -x);
            st.range_update(l - 1, r, x);
        } else if (op == 2) {
            int l, r;
            cin >> l >> r;

            st.range_update(l - 1, r);
        } else {
            int i;
            cin >> i;

            auto level = fw.pref_sum(i) + a[i];
            auto [value, reduce, _] = st.range_query(i - 1, i);
            if (reduce) level = (level - value) % p + value;
            cout << level << "\n";
        }
    }
}