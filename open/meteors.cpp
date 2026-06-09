#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        pair<long long, int> v1, v2;

        Monoid() : v1({1e18, -1}), v2({1e18, -1}) {}

        auto & operator=(const pair<pair<long long, long long>, int> &v) {
            v1 = {v.first.first, v.second};
            v2 = {v.first.second, v.second};
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            v1 = min(v1, monoid.v1);
            v2 = min(v2, monoid.v2);
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

    void point_update(int i, const pair<long long, long long> &v) {
        for (ST[i += n] = {v, i}; i > 1; i >>= 1) pull(i >> 1);
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

    PURQSegmentTree(int n, const vector<pair<long long, long long>> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = {a[i], i};
        build();
    }
};

struct SegmentTreeBeats {
    struct Monoid {
        long long xl, xr;
        pair<long long, int> inactive, delta;

        Monoid() : xl(-1e18), xr(-1e18), inactive({1e18, -1}), delta({1e18, -1}) {}

        auto & operator=(const pair<long long, int> &v) {
            inactive = v;
            delta = {inactive.first - xr, inactive.second};
            return *this;
        }

        auto & operator+=(const long long &v) {
            xl = xr = v;
            delta = {inactive.first - xr, inactive.second};
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            xl = min(xl, monoid.xl);
            xr = max(xr, monoid.xr);
            inactive = min(inactive, monoid.inactive);
            delta = min(delta, monoid.delta);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;
    vector<long long> lazy;

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const long long &v) {
        ST[i] += v;
        if (i < n) lazy[i] = max(lazy[i], v);
    }

    void push(int i) {
        if (lazy[i] != -1e18) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = -1e18;
        }
    }

    void point_update(int pos, const long long &v) {
        point_update(1, pos, v, 0, n);
    }

    void point_update(int i, int pos, const long long &v, int l, int r) {
        if (l + 1 == r) {
            ST[i] = {v, pos};
            return;
        }

        push(i);

        int m = midpoint(l, r);
        if (pos < m) point_update(i << 1, pos, v, l, m);
        else point_update(i << 1 | 1, pos, v, m, r);

        pull(i);
    }

    void range_update(int l, int r, const long long &v) {
        range_update(1, l, r, v, 0, n);
    }

    void range_update(int i, int ql, int qr, const long long &v, int l, int r) {
        if (qr <= l || r <= ql || ST[i].xl >= v) return;
        if (ql <= l && r <= qr && ST[i].xr <= v) {
            apply(i, v);
            return;
        }

        push(i);

        int m = midpoint(l, r);
        range_update(i << 1, ql, qr, v, l, m);
        range_update(i << 1 | 1, ql, qr, v, m, r);

        pull(i);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTreeBeats(int n, const vector<long long> &a) : n(n), ST(2 * n), lazy(n, -1e18) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = {a[i], i};
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> meteors(n);
    for (auto &[t, x] : meteors) cin >> t >> x;
    sort(meteors.begin(), meteors.end());

    vector<pair<long long, long long>> endpoints(n);
    for (int i = 0; i < n; i++) {
        auto [t, x] = meteors[i];
        endpoints[i] = {x - t, x + t};
    }

    vector<pair<int, int>> intervals;
    vector<int> pos(n);
    for (int l = 0, r = 1; l < n; l = r++) {
        for (; r < n && meteors[l].first == meteors[r].first; r++);
        intervals.emplace_back(l, r);
        for (int k = l; k < r; k++) pos[k] = intervals.size() - 1;
    }

    int m = intervals.size();
    vector<long long> l_min(m, LLONG_MAX), r_min(m, LLONG_MAX);
    for (int i = 0; i < n; i++) {
        int p = pos[i];
        l_min[p] = min(l_min[p], endpoints[i].first);
        r_min[p] = min(r_min[p], endpoints[i].second);
    }

    vector<bool> visited(n, false);
    PURQSegmentTree st(n, endpoints);
    SegmentTreeBeats st_l(bit_ceil((unsigned) m), l_min), st_r(bit_ceil((unsigned) m), r_min);
    auto dist = LLONG_MAX, sweep = 2 * st[1].v2.first;
    while (dist > sweep) {
        auto update = [&](int i) {
            if (visited[i]) return;
            visited[i] = true;
            st.point_update(i, {1e18, 1e18});
            int p = pos[i];
            auto [l, r] = intervals[p];
            auto [v1, v2] = st.range_query(l, r);
            st_l.point_update(p, v1.first);
            st_l.range_update(0, p + 1, endpoints[i].first);
            st_r.point_update(p, v2.first);
            st_r.range_update(p, m, endpoints[i].second);
            dist = min(dist, max(sweep, -2 * endpoints[i].first));
        };

        for (;;) {
            if (2 * st[1].v2.first <= sweep) update(st[1].v2.second);
            else if (st_l[1].delta.first <= sweep) {
                int p = st_l[1].delta.second;
                auto [l, r] = intervals[p];
                auto [v1, v2] = st.range_query(l, r);
                update(v1.second);
            } else if (st_r[1].delta.first <= sweep) {
                int p = st_r[1].delta.second;
                auto [l, r] = intervals[p];
                auto [v1, v2] = st.range_query(l, r);
                update(v2.second);
            } else break;
        }
        sweep = min({2 * st[1].v2.first, st_l[1].delta.first, st_r[1].delta.first});
    }
    cout << dist / 2 << (dist & 1 ? ".5" : ".0");
}
