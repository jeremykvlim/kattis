#include <bits/stdc++.h>
using namespace std;

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

    FenwickTree(int n) : BIT(n, 0) {}
};

struct SegmentTree {
    struct Segment {
        int value;

        Segment() : value(0) {}

        auto & operator=(const int &v) {
            value = v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            value = max(value, seg.value);
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

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Segment range_query(int l, int r) {
        Segment seg;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) seg += ST[l++];
            if (r & 1) seg += ST[--r];
        }

        return seg;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    vector<int> a(n + 1);
    vector<set<int>> indices(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        indices[a[i]].emplace(i);
    }

    vector<int> count(n + 1), l(n + 1), r(n + 1);
    vector<set<int>::iterator> it_l(n + 1), it_r(n + 1);
    FenwickTree<int> fw_l1(n + 1), fw_r1(n + 1), fw_l2(n + 1), fw_r2(n + 1);
    SegmentTree st(n);

    auto calc = [&](int t) {
        count[t] = indices[t].size();
        if (count[t] >= 2) {
            auto it1 = indices[t].begin();
            l[t] = *it1;
            it_l[t] = next(it1);
            auto it2 = prev(indices[t].end());
            r[t] = *it2;
            it_r[t] = prev(it2);

            fw_l1.update(l[t], count[t] - 1);
            fw_r1.update(r[t], count[t] - 1);
            fw_l2.update(*it_l[t], 1);
            fw_r2.update(*it_r[t], 1);
            st.assign(l[t] - 1, r[t]);
        } else {
            if (count[t] == 1) l[t] = r[t] = *indices[t].begin();
            else {
                l[t] = n + 1;
                r[t] = 0;
            }
        }
    };

    for (int t = 1; t <= n; t++) calc(t);

    auto update = [&](int t) {
        if (count[t] >= 2) {
            fw_l1.update(l[t], 1 - count[t]);
            fw_r1.update(r[t], 1 - count[t]);
            fw_l2.update(*it_l[t], -1);
            fw_r2.update(*it_r[t], -1);
            st.assign(l[t] - 1, 0);
        }

        calc(t);
    };

    while (Q--) {
        int q, t;
        cin >> q >> t;

        if (q == 1) {
            int i;
            cin >> i;

            if (a[i] != t) {
                indices[a[i]].erase(i);
                update(a[i]);
                indices[t].emplace(i);
                update(t);
                a[i] = t;
            }
        } else {
            if (count[t] < 2 ||
                l[t] > 1 && fw_l2.pref_sum(l[t] - 1) ||
                r[t] < n && fw_r2.pref_sum(n) > fw_r2.pref_sum(r[t]) ||
                l[t] > 1 && st.range_query(0, l[t] - 1).value > r[t]) {
                cout << "-1\n";
                continue;
            }

            cout << (l[t] > 1 ? fw_l1.pref_sum(l[t] - 1) - fw_l1.pref_sum(0) : 0) + (r[t] < n ? fw_r1.pref_sum(n) - fw_r1.pref_sum(r[t]) : 0) << "\n";
        }
    }
}
