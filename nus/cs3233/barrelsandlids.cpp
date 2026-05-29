#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        long long value;

        Monoid() : value(1e18) {}

        auto & operator=(const long long &v) {
            value = v;
            return *this;
        }

        auto & operator+=(const long long &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            value = min(value, monoid.value);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<long long> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const long long &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        if (lazy[i]) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = 0;
        }
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

    void range_update(int l, int r, const long long &v) {
        l += n;
        r += n;
        push_down(l, r);
        
        int temp_l = l, temp_r = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, v);
            if (r & 1) apply(--r, v);
        }
        
        pull_up(temp_l, temp_r);
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

    RURQSegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), b(n), deduped;
    for (int &ai : a) {
        cin >> ai;

        deduped.emplace_back(ai);
    }
    for (int &bi : b) {
        cin >> bi;

        deduped.emplace_back(bi);
    }
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

    auto a_sorted = a, b_sorted = b;
    sort(a_sorted.begin(), a_sorted.end());
    sort(b_sorted.begin(), b_sorted.end());
    for (int i = 0; i < n; i++)
        if (a_sorted[i] < b_sorted[i]) {
            cout << -1;
            exit(0);
        }

    int m = deduped.size();
    vector<int> c(m);
    for (int i = 0, l = 0, r = 0; i < m; i++) {
        for (; l < n && a_sorted[l] < deduped[i]; l++);
        for (; r < n && b_sorted[r] < deduped[i]; r++);
        c[i] = r - l;
    }

    int count = 0;
    vector<pair<int, int>> intervals;
    for (int i = 0; i < n; i++)
        if (a[i] == b[i]) count++;
        else if (a[i] > b[i]) intervals.emplace_back(upper_bound(deduped.begin(), deduped.end(), b[i]) - deduped.begin(), upper_bound(deduped.begin(), deduped.end(), a[i]) - deduped.begin());
    sort(intervals.begin(), intervals.end(), [&](auto &p1, auto &p2) { return p1.second != p2.second ? p1.second < p2.second : p1.first > p2.first; });

    RURQSegmentTree st(bit_ceil((unsigned) m), c);
    for (auto [l, r] : intervals)
        if (st.range_query(l, r).value > 0) {
            count++;
            st.range_update(l, r, -1);
        }
    cout << count;
}