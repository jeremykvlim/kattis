#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long value;
        int i;

        Segment() : value(-1), i(-1) {}

        auto & operator=(const int &index) {
            value = 0;
            i = index;
            return *this;
        }

        auto & operator+=(const long long &v) {
            value += v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            if (value <= seg.value) {
                value = seg.value;
                i = seg.i;
            }
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n;
    vector<Segment> ST;
    vector<long long> lazy;

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
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

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void modify(const int &pos, const long long &v) {
        modify(1, pos, v, 0, n);
    }

    void modify(int i, const int &pos, const long long &v, int l, int r) {
        if (l + 1 == r) {
            apply(i, v);
            return;
        }

        push(i);

        int m = midpoint(l, r);
        if (pos < m) modify(i << 1, pos, v, l, m);
        else modify(i << 1 | 1, pos, v, m, r);

        pull(i);
    }

    void modify(int l, int r, long long v) {
        modify(1, l, r, v, 0, n);
    }

    void modify(int i, int ql, int qr, long long v, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            apply(i, v);
            return;
        }

        push(i);

        int m = midpoint(l, r);
        modify(i << 1, ql, qr, v, l, m);
        modify(i << 1 | 1, ql, qr, v, m, r);

        pull(i);
    }

    Segment range_query(int l, int r) {
        return range_query(1, l, r, 0, n);
    }

    Segment range_query(int i, int ql, int qr, int l, int r) {
        if (qr <= l || r <= ql) return {};
        if (ql <= l && r <= qr) return ST[i];

        push(i);

        int m = midpoint(l, r);
        return range_query(i << 1, ql, qr, l, m) + range_query(i << 1 | 1, ql, qr, m, r);
    }

    SegmentTree(int n, const vector<int> &a) : n(n), ST(2 * n), lazy(n) {
        int m = bit_ceil(a.size());
        for (int i = 0; i < a.size(); i++) ST[(i + m) % n + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> C(n + 1);
    vector<long long> v(n + 1);
    for (int i = 1; i <= n; i++) cin >> C[i] >> v[i];

    vector<vector<int>> indices(m + 1);
    for (int i = 1; i <= n; i++) indices[C[i]].emplace_back(i);

    vector<int> before(n + 1), after(n + 1);
    for (int c = 1; c <= m; c++) {
        stack<int> s;
        for (int i = 0; i < indices[c].size(); i++) {
            int j = indices[c][i];
            while (!s.empty() && v[indices[c][s.top()]] <= v[j]) s.pop();
            before[j] = s.empty() ? 0 : indices[c][s.top()];
            s.emplace(i);
        }
        while (!s.empty()) s.pop();
        for (int i = indices[c].size() - 1; ~i; i--) {
            int j = indices[c][i];
            while (!s.empty() && v[indices[c][s.top()]] <= v[j]) s.pop();
            after[j] = s.empty() ? n + 1 : indices[c][s.top()];
            s.emplace(i);
        }
    }
    vector<vector<int>> updates(n + 1);
    for (int i = 1; i <= n; i++) updates[before[i]].emplace_back(i);

    priority_queue<pair<int, int>> pq;
    for (int c = 1; c <= m; c++) pq.emplace(n + 1, c);
    vector<int> seen(m + 1, n + 1), end(n + 1, 0);
    for (int i = n; i; i--) {
        seen[C[i]] = i;
        pq.emplace(i, C[i]);
        while (!end[i]) {
            auto [j, cj] = pq.top();

            if (seen[cj] != j) pq.pop();
            else end[i] = j;
        }
    }

    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    SegmentTree st(n, p);

    vector<long long> dp(n + 2, 0), base(n + 1, 0);
    vector<int> r(n + 2, -1);
    for (int i = n; i; i--) {
        dp[i] = dp[i + 1];
        for (int j : updates[i]) st.modify(j - 1, after[j] - 1, -v[j]);
        st.modify(i - 1, after[i] - 1, v[i]);

        auto [value, k] = st.range_query(end[i] - 1, n);
        if (dp[i] < value) {
            dp[i] = value;
            r[i] = k;
        }

        if (i > 1) {
            st.modify(i - 2, dp[i] - base[i - 1]);
            base[i - 1] = dp[i];
        }
    }

    string s(n, 'X');
    vector<long long> temp(m + 1, 0);
    vector<int> id(m + 1, -1);
    for (int i = 1; i <= n; i++) {
        if (~r[i]) {
            auto undo1 = temp;
            auto undo2 = id;

            for (int j = i; j <= r[i]; j++) {
                int c = C[j];
                if (temp[c] < v[j]) {
                    temp[c] = v[j];
                    id[c] = j - 1;
                }
            }

            for (int c = 1; c <= m; c++) s[id[c]] = 'O';
            tie(temp, id) = tie(undo1, undo2);
            i = r[i];
        }
    }

    cout << s;
}
