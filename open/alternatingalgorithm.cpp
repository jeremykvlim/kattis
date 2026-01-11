#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        int count, r;

        Monoid() : count(0), r(0) {}

        auto & operator=(const pair<int, int> &v) {
            tie(count, r) = v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            r = max(r + monoid.count, monoid.r - count);
            count += monoid.count;
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

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n) : n(n), ST(2 * n) {}
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
    PURQSegmentTree st(n + 1);
    for (int i = 0, j = 0; i <= n; i++) {
        st.point_update(sorted[i].second, {1, sorted[i].second + !(sorted[i].second & 1)});
        if (i < n && sorted[i].first == sorted[i + 1].first) continue;
        for (; j <= n && a[j].first <= sorted[i].first; j++);
        rounds = max(rounds, st.range_query(j, n + 1).r - j);
    }
    cout << rounds;
}
