#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
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

        auto & operator+=(const Segment &seg) {
            if (value <= seg.value) return seg;
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n, h;
    vector<Segment> ST;
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
        for (int k = h; k; k--) {
            int j = i >> k;
            if (lazy[j]) {
                apply(j << 1, lazy[j]);
                apply(j << 1 | 1, lazy[j]);
                lazy[j] = 0;
            }
        }
    }

    void point_update(int i, const long long &v) {
        for (apply(i += n, v); i > 1; i >>= 1) pull(i >> 1);
    }

    void range_update(int l, int r, const long long &v) {
        push(l + n);
        push(r + n - 1);
        bool cl = false, cr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (cl) pull(l - 1);
            if (cr) pull(r);
            if (l & 1) {
                cl = true;
                apply(l++, v);
            }
            if (r & 1) {
                cr = true;
                apply(--r, v);
            }
        }

        for (l--; r; l >>= 1, r >>= 1) {
            if (cl) pull(l);
            if (cr && (!cl || l < r)) pull(r);
        }
    }

    Segment range_query(int l, int r) {
        push(l + n);
        push(r + n - 1);
        Segment sl, sr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = sl + ST[l++];
            if (r & 1) sr = ST[--r] + sr;
        }

        return sl + sr;
    }

    RURQSegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
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
    RURQSegmentTree st(bit_ceil((unsigned) n), p);

    vector<long long> dp(n + 2, 0), base(n + 1, 0);
    vector<int> r(n + 2, -1);
    for (int i = n; i; i--) {
        dp[i] = dp[i + 1];
        for (int j : updates[i]) st.range_update(j - 1, after[j] - 1, -v[j]);
        st.range_update(i - 1, after[i] - 1, v[i]);

        auto [value, k] = st.range_query(end[i] - 1, n);
        if (dp[i] < value) {
            dp[i] = value;
            r[i] = k;
        }

        if (i > 1) {
            st.point_update(i - 2, dp[i] - base[i - 1]);
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