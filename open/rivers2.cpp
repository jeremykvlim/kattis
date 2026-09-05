#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        array<long long, 7> sums;
        long long count, sum_i, sum_sq, first;

        Monoid() : sums{}, count(0), sum_i(0), sum_sq(0), first(0) {}

        auto & operator+=(const Monoid &monoid) {
            sums[0] += monoid.sums[0];
            sums[1] += monoid.sums[1];
            sums[2] += monoid.sums[2];
            sums[3] += monoid.sums[3];
            sums[6] += monoid.sums[6];

            if (!count && monoid.count) {
                sums[4] = monoid.sums[4];
                sums[5] = monoid.sums[5];
                first = monoid.first;
            }

            count += monoid.count;
            sum_i += monoid.sum_i;
            sum_sq += monoid.sum_sq;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<array<long long, 4>> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, long long c0, long long c1, int b) {
        ST[i].sums[6] += c0 * ST[i].sums[b ^ 1] + c1 * ST[i].sums[(b ^ 1) + 2];
        ST[i].sums[b] += c0 * ST[i].count + c1 * ST[i].sum_i;
        ST[i].sums[b + 2] += c0 * ST[i].sum_i + c1 * ST[i].sum_sq;
        ST[i].sums[b + 4] += c0 + c1 * ST[i].first;
        if (i < n) {
            lazy[i][b] += c0;
            lazy[i][b + 2] += c1;
        }
    }

    void push(int i) {
        auto [v0, h0, v1, h1] = lazy[i];
        if (!v0 && !h0 && !v1 && !h1) return;

        if (v0 || v1) {
            apply(i << 1, v0, v1, 0);
            apply(i << 1 | 1, v0, v1, 0);
        }
        if (h0 || h1) {
            apply(i << 1, h0, h1, 1);
            apply(i << 1 | 1, h0, h1, 1);
        }
        lazy[i] = {0, 0, 0, 0};
    }

    void push_down(int l, int r) {
        for (int b = h; b; b--) {
            if (((l >> b) << b) != l) push(l >> b);
            if (((r >> b) << b) != r) push((r - 1) >> b);
        }
    }

    void pull_up(int l, int r) {
        for (int b = 1; b <= h; b++) {
            if (((l >> b) << b) != l) pull(l >> b);
            if (((r >> b) << b) != r) pull((r - 1) >> b);
        }
    }

    void point_update(int p, long long c0, long long c1, int b) {
        range_update(p - 1, p, c0, c1, b);
    }

    void range_update(int l, int r, long long c0, long long c1, int b) {
        l += n;
        r += n;
        push_down(l, r);

        int temp_l = l, temp_r = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, c0, c1, b);
            if (r & 1) apply(--r, c0, c1, b);
        }

        pull_up(temp_l, temp_r);
    }

    Monoid point_query(int p) {
        return range_query(p, p + 1);
    }

    Monoid range_query(int l, int r) {
        l += n;
        r += n;
        push_down(l, r);

        Monoid ml, mr;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    array<long long, 4> prefix_query(long long c, long long extra_v, long long extra_h) {
        array<long long, 4> a{0, 0, ST[1].sums[4], ST[1].sums[5]};
        long long sum_v = 0, sum_h = 0, sum_both = 0, base = extra_v * extra_h, v = 0, h = 0;
        int i = 1, l = 0, r = n;
        while (i < n) {
            push(i);

            int m = l + (r - l) / 2;
            auto cost = sum_both + ST[i << 1].sums[6] +
                        extra_v * (sum_h + ST[i << 1].sums[1]) +
                        extra_h * (sum_v + ST[i << 1].sums[0]) +
                        (base - (ST[i << 1 | 1].sums[4] + extra_v) * (ST[i << 1 | 1].sums[5] + extra_h)) * m;

            if (c < cost) {
                v = ST[i << 1 | 1].sums[4];
                h = ST[i << 1 | 1].sums[5];
                i <<= 1;
                r = m;
            } else {
                sum_v += ST[i << 1].sums[0];
                sum_h += ST[i << 1].sums[1];
                sum_both += ST[i << 1].sums[6];
                a = {m, cost, ST[i << 1 | 1].sums[4], ST[i << 1 | 1].sums[5]};
                (i <<= 1) |= 1;
                l = m;
            }
        }

        auto cost = sum_both + ST[i].sums[6] +
                    extra_v * (sum_h + ST[i].sums[1]) +
                    extra_h * (sum_v + ST[i].sums[0]) +
                    (base - (v + extra_v) * (h + extra_h)) * (l + 1);

        if (c >= cost) a = {l + 1, cost, v, h};
        return a;
    }

    RURQSegmentTree(int n) : n(n), h(__lg(n)), ST(2 * n), lazy(n, {0, 0, 0, 0}) {
        for (int i = 0; i < n; i++) {
            ST[i + n].count = 1;
            ST[i + n].sum_i = ST[i + n].first = i + 1;
            ST[i + n].sum_sq = (long long) (i + 1) * (i + 1);
        }
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, z;
    string s, t;
    cin >> n >> m >> z >> s >> t;

    auto total = (long long) n * m;
    int k = bit_ceil((unsigned) max(n, m) + 1);
    RURQSegmentTree st(k);

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
                auto monoid = st.point_query(i + 1);
                auto v2 = monoid.sums[0], h2 = monoid.sums[1];
                a2 = (v2 + extra_v) * (h2 + extra_h);
            }
            cout << total - a1 + min(a1 - a2, (c - cost) / (i + 1)) << "\n";
        }
    }
}
