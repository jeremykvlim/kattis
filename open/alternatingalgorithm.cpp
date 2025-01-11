#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        int count, r;

        Segment() : count(0), r(0) {}

        auto & operator=(const pair<int, int> &v) {
            tie(count, r) = v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            r = max(r + seg.count, seg.r - count);
            count += seg.count;
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

    void assign(int i, const pair<int, int> &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto range_query(int l, int r) {
        Segment sl, sr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = sl + ST[l++];
            if (r & 1) sr = ST[--r] + sr;
        }

        return sl + sr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> a(n + 1);
    for (int i = 0; i <= n; i++) {
        cin >> a[i].first;

        a[i].second = i;
    }

    auto sorted = a;
    sort(sorted.begin(), sorted.end());

    int rounds = 0;
    SegmentTree st(n + 1);
    for (int i = 0, j = 0; i <= n; i++) {
        st.assign(sorted[i].second, {1, sorted[i].second + !(sorted[i].second & 1)});
        if (i < n && sorted[i].first == sorted[i + 1].first) continue;
        while (j <= n && a[j].first <= sorted[i].first) j++;
        rounds = max(rounds, st.range_query(j, n + 1).r - j);
    }
    cout << rounds;
}
