#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Monoid {
        array<long long, 7> sums;

        Monoid() : sums{} {}

        auto & operator+=(const Monoid &monoid) {
            sums[0] += monoid.sums[0];
            sums[1] += monoid.sums[1];
            sums[2] += monoid.sums[2];
            sums[3] += monoid.sums[3];
            sums[6] += monoid.sums[6];
            return *this;
        }

        friend auto operator+(Monoid sl, const Monoid &sr) {
            return sl += sr;
        }
    };

    int n;
    vector<Monoid> ST;
    vector<array<long long, 4>> lazy;

    SegmentTree(int n) : n(n), ST(2 * n), lazy(2 * n, {0, 0, 0, 0}) {}

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, int l, int r, long long c0, long long c1, int b) {
        auto sum_of_sq = [&](long long n) {
            return n * (n + 1) * (2 * n + 1) / 6;
        };

        auto sum_i = ((long long) l + r + 1) * (r - l) / 2;
        ST[i].sums[6] += c0 * ST[i].sums[b ^ 1] + c1 * ST[i].sums[(b ^ 1) + 2];
        ST[i].sums[b] += c0 * (r - l) + c1 * sum_i;
        ST[i].sums[b + 2] += c0 * sum_i + c1 * (sum_of_sq(r) - sum_of_sq(l));
        ST[i].sums[b + 4] += c0 + c1 * (l + 1);
        lazy[i][b] += c0;
        lazy[i][b + 2] += c1;
    }

    void push(int i, int l, int r) {
        auto [v0, h0, v1, h1] = lazy[i];
        if (!v0 && !v1 && !h0 && !h1) return;

        int m = midpoint(l, r);
        if (v0 || v1) {
            apply(i << 1, l, m, v0, v1, 0);
            apply(i << 1 | 1, m, r, v0, v1, 0);
        }
        if (h0 || h1) {
            apply(i << 1, l, m, h0, h1, 1);
            apply(i << 1 | 1, m, r, h0, h1, 1);
        }
        lazy[i] = {0, 0, 0, 0};
    }

    void point_update(int p, long long c0, long long c1, int b) {
        range_update(p - 1, p, c0, c1, b);
    }

    void range_update(int ql, int qr, long long c0, long long c1, int b) {
        range_update(1, ql, qr, c0, c1, b, 0, n);
    }

    void range_update(int i, int ql, int qr, long long c0, long long c1, int b, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            apply(i, l, r, c0, c1, b);
            return;
        }

        push(i, l, r);

        int m = midpoint(l, r);
        range_update(i << 1, ql, qr, c0, c1, b, l, m);
        range_update(i << 1 | 1, ql, qr, c0, c1, b, m, r);

        pull(i);
    }

    array<long long, 2> point_query(int pos) {
        return point_query(1, pos, 0, n);
    }

    array<long long, 2> point_query(int i, int pos, int l, int r) {
        if (l + 1 == r) return {ST[i].sums[0], ST[i].sums[1]};

        push(i, l, r);
        int m = midpoint(l, r);
        if (pos < m) return point_query(i << 1, pos, l, m);
        return point_query(i << 1 | 1, pos, m, r);
    }

    array<long long, 4> prefix_query(long long c, long long extra_v, long long extra_h) {
        array<long long, 4> a{0, 0, ST[1].sums[4], ST[1].sums[5]};
        long long sum_v = 0, sum_h = 0, sum_both = 0, base = extra_v * extra_h;
        prefix_query(1, 0, n, c, extra_v, extra_h, 0, 0, base, sum_v, sum_h, sum_both, a);
        return a;
    }

    void prefix_query(int i, int l, int r, long long c, long long extra_v, long long extra_h, long long v, long long h, long long base,
                      long long &sum_v, long long &sum_h, long long &sum_both, array<long long, 4> &a) {
        if (l + 1 == r) {
            auto cost = sum_both + ST[i].sums[6] + extra_v * (sum_h + ST[i].sums[1]) + extra_h * (sum_v + ST[i].sums[0]) + 
                        (base - (v + extra_v) * (h + extra_h)) * (l + 1);
            if (c >= cost) {
                sum_v += ST[i].sums[0];
                sum_h += ST[i].sums[1];
                sum_both += ST[i].sums[6];
                a = {l + 1, cost, v, h};
            }
            return;
        }

        push(i, l, r);

        int m = midpoint(l, r);
        auto cost = sum_both + ST[i << 1].sums[6] + extra_v * (sum_h + ST[i << 1].sums[1]) + extra_h * (sum_v + ST[i << 1].sums[0]) +
                    (base - (ST[i << 1 | 1].sums[4] + extra_v) * (ST[i << 1 | 1].sums[5] + extra_h)) * m;

        if (c < cost) prefix_query(i << 1, l, m, c, extra_v, extra_h, ST[i << 1 | 1].sums[4], ST[i << 1 | 1].sums[5], base, sum_v, sum_h, sum_both, a);
        else {
            sum_v += ST[i << 1].sums[0];
            sum_h += ST[i << 1].sums[1];
            sum_both += ST[i << 1].sums[6];
            a = {m, cost, ST[i << 1 | 1].sums[4], ST[i << 1 | 1].sums[5]};
            prefix_query(i << 1 | 1, m, r, c, extra_v, extra_h, v, h, base, sum_v, sum_h, sum_both, a);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, z;
    string s, t;
    cin >> n >> m >> z >> s >> t;

    auto total = (long long) n * m;
    int k = max(n, m) + 1;
    SegmentTree st(k);

    auto handle_boundary = [&](int len, int sgn, bool b) {
        if (len) st.range_update(0, len, sgn * (len + 1), -sgn, b);
    };

    auto handle_gap = [&](int l, int sgn, bool b) {
        int len = l / 2;
        if (len) st.range_update(0, len, sgn * (l + 2), -2 * sgn, b);
        if (l & 1) st.point_update(len + 1, sgn, 0, b);
    };

    set<int> v, h;
    auto init = [&](string &s, set<int> &indices, bool b) {
        for (int i = 1; i <= s.size(); i++)
            if (s[i - 1] == '+') indices.emplace(i);
        if (indices.empty()) return;
        handle_boundary(*indices.begin() - 1, 1, b);
        handle_boundary(s.size() - *indices.rbegin(), 1, b);
        for (auto it = next(indices.begin()); it != indices.end(); it++) handle_gap(*it - *prev(it) - 1, 1, b);
    };
    init(s, v, false);
    init(t, h, true);

    while (z--) {
        char q;
        cin >> q;

        auto toggle = [&](int len, int i, set<int> &indices, bool b) {
            auto it = indices.find(i);
            if (it != indices.end()) {
                int l = it == indices.begin() ? 0 : *prev(it);
                int r = next(it) == indices.end() ? 0 : *next(it);
                if (!l && !r) {
                    handle_boundary(i - 1, -1, b);
                    handle_boundary(len - i, -1, b);
                } else if (!l) {
                    handle_boundary(i - 1, -1, b);
                    handle_gap(r - i - 1, -1, b);
                    handle_boundary(r - 1, 1, b);
                } else if (!r) {
                    handle_boundary(len - i, -1, b);
                    handle_gap(i - l - 1, -1, b);
                    handle_boundary(len - l, 1, b);
                } else {
                    handle_gap(i - l - 1, -1, b);
                    handle_gap(r - i - 1, -1, b);
                    handle_gap(r - l - 1, 1, b);
                }
                indices.erase(it);
            } else {
                it = indices.lower_bound(i);
                int l = it == indices.begin() ? 0 : *prev(it);
                int r = it == indices.end() ? 0 : *it;
                if (!l && !r) {
                    handle_boundary(i - 1, 1, b);
                    handle_boundary(len - i, 1, b);
                } else if (!l) {
                    handle_boundary(r - 1, -1, b);
                    handle_boundary(i - 1, 1, b);
                    handle_gap(r - i - 1, 1, b);
                } else if (!r) {
                    handle_boundary(len - l, -1, b);
                    handle_boundary(len - i, 1, b);
                    handle_gap(i - l - 1, 1, b);
                } else {
                    handle_gap(r - l - 1, -1, b);
                    handle_gap(i - l - 1, 1, b);
                    handle_gap(r - i - 1, 1, b);
                }
                indices.emplace(i);
            }
        };

        if (q == 'N') {
            int i;
            cin >> i;

            toggle(n, i, v, false);
        } else if (q == 'M') {
            int i;
            cin >> i;

            toggle(m, i, h, true);
        } else {
            long long c;
            cin >> c;

            long long extra_v = v.empty() ? n : 0, extra_h = h.empty() ? m : 0;

            auto [i, cost, v1, h1] = st.prefix_query(c, extra_v, extra_h);
            long long a1 = (v1 + extra_v) * (h1 + extra_h), a2;
            if (i + 2 > k) a2 = extra_v * extra_h;
            else {
                auto [v2, h2] = st.point_query(i + 1);
                a2 = (v2 + extra_v) * (h2 + extra_h);
            }
            cout << total - a1 + min(a1 - a2, (c - cost) / (i + 1)) << "\n";
        }
    }
}
