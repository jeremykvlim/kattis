#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long value;

        Segment() : value(-1e18) {}

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

    void assign(int i, const long long &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto range_query(int l, int r) {
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

    int n, m;
    cin >> n >> m;

    vector<long long> p(m + 1), r(m + 1);
    for (int i = 1; i <= m; i++) cin >> p[i];
    for (int i = 1; i <= m; i++) cin >> r[i];

    vector<pair<int, long long>> customers(n);
    for (auto &[c, t] : customers) cin >> c >> t;

    vector<long long> times{0};
    for (auto [c, t] : customers) times.emplace_back(t + r[c]);
    sort(times.begin(), times.end());
    times.erase(unique(times.begin(), times.end()), times.end());
    int k = times.size();

    SegmentTree st(k);
    vector<long long> dp1(m + 1, -1e18), dp2(k, -1e18);
    int i = lower_bound(times.begin(), times.end(), 0) - times.begin();
    dp1[0] = dp2[i] = 0;
    st.assign(i, 0);
    for (auto [c, t] : customers) {
        i = upper_bound(times.begin(), times.end(), t - p[c]) - times.begin();
        auto v = max(st.range_query(0, i).value, dp1[c]) + 1;
        if (v < 0) continue;

        i = lower_bound(times.begin(), times.end(), t + r[c]) - times.begin();
        dp1[c] = max(dp1[c], v);
        dp2[i] = max(dp2[i], v);
        st.assign(i, dp2[i]);
    }
    cout << *max_element(dp1.begin(), dp1.end());
}
