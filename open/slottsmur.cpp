#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    int n;
    vector<T> BIT;

    FenwickTree(int n) : n(n), BIT(n, 0) {}

    void update(int i, T v) {
        for (; i < n; i += i & -i) BIT[i] += v;
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
};

struct PURQSegmentTree {
    struct Monoid {
        int len, pos_l, pos_r;
        long long h_max, pref_jump, suff_jump;

        Monoid() : len(0), pos_l(-1), pos_r(-1), h_max(-1), pref_jump(0), suff_jump(0) {}

        auto & operator=(const pair<int, int> &v) {
            len = 1;
            h_max = v.first;
            pos_l = pos_r = v.second;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (!len) return *this = monoid;
            if (!monoid.len) return *this;

            len += monoid.len;
            if (h_max < monoid.h_max) {
                h_max = monoid.h_max;
                pos_l = monoid.pos_l;
                pos_r = monoid.pos_r;
            } else if (h_max == monoid.h_max) pos_r = monoid.pos_r;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;

    long long pref_eval(int i, long long h_bound) {
        if (h_bound >= ST[i].h_max) return h_bound * ST[i].len;

        auto sum = 0LL;
        while (i < n)
            if (h_bound < ST[i << 1].h_max) {
                sum += ST[i].pref_jump;
                i = i << 1;
            } else {
                sum += h_bound * ST[i << 1].len;
                i = i << 1 | 1;
                if (h_bound >= ST[i].h_max) return sum + h_bound * ST[i].len;
            }
        return sum + ST[i].h_max;
    }

    long long suff_eval(int i, long long h_bound) {
        if (h_bound >= ST[i].h_max) return h_bound * ST[i].len;

        auto sum = 0LL;
        while (i < n)
            if (h_bound < ST[i << 1 | 1].h_max) {
                sum += ST[i].suff_jump;
                i = i << 1 | 1;
            } else {
                sum += h_bound * ST[i << 1 | 1].len;
                i = i << 1;
                if (h_bound >= ST[i].h_max) return sum + h_bound * ST[i].len;
            }
        return sum + ST[i].h_max;
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
        ST[i].pref_jump = pref_eval(i << 1 | 1, ST[i << 1].h_max);
        ST[i].suff_jump = suff_eval(i << 1, ST[i << 1 | 1].h_max);
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const pair<int, int> &v) {
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

    vector<int> walk(int l, int r) {
        vector<int> left, right;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) left.emplace_back(l++);
            if (r & 1) right.emplace_back(--r);
        }
        left.insert(left.end(), right.rbegin(), right.rend());
        return left;
    }

    long long pref_sum(int l, int r) {
        auto h_bound = 0LL;
        auto path = walk(l - 1, r);
        return accumulate(path.begin(), path.end(), 0LL, [&](long long sum, int v) {
            sum += pref_eval(v, h_bound);
            h_bound = max(h_bound, ST[v].h_max);
            return sum;
        });
    }

    long long suff_sum(int l, int r) {
        auto h_bound = 0LL;
        auto path = walk(l - 1, r);
        return accumulate(path.rbegin(), path.rend(), 0LL, [&](long long sum, int v) {
            sum += suff_eval(v, h_bound);
            h_bound = max(h_bound, ST[v].h_max);
            return sum;
        });
    }

    PURQSegmentTree(int n, const vector<pair<int, int>> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < n; i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> h(n + 1);
    FenwickTree<long long> fw(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> h[i];

        fw.update(i, h[i]);
    }

    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; i++) a[i] = {h[i + 1], i + 1};
    PURQSegmentTree st(bit_ceil((unsigned) n), a);
    while (q--) {
        int t;
        cin >> t;

        if (t == 1) {
            int l, r;
            cin >> l >> r;

            auto [_, pos_l, pos_r, h_max, __, ___] = st.range_query(l - 1, r);
            cout << st.pref_sum(l, pos_r) + st.suff_sum(pos_l, r) - (pos_r - pos_l + 1) * h_max - fw.range_sum_query(l - 1, r) << "\n";
        } else {
            int i, w;
            cin >> i >> w;

            h[i] += w;
            fw.update(i, w);
            st.point_update(i - 1, {h[i], i});
        }
    }
}
