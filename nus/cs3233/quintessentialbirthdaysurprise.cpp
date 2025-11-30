#include <bits/stdc++.h>
using namespace std;

struct LiChaoSegmentTree {
    struct Function {
        long long m, c;
        int i;

        Function(long long m = 0, long long c = -1e10, int i = -1) : m(m), c(c), i(i) {}

        long long operator()(long long x) {
            return m * x + c;
        }
    };
    int n;
    vector<long long> xs;
    vector<Function> ST;

    void insert(Function f) {
        insert(1, 0, n, f);
    }

    void insert(int i, int l, int r, Function f) {
        bool left = f(xs[l]) > ST[i](xs[l]);
        if (l + 1 == r) {
            if (left) ST[i] = f;
            return;
        }

        int m = midpoint(l, r);
        bool mid = f(xs[m]) > ST[i](xs[m]);
        if (mid) swap(f, ST[i]);

        if (left != mid) insert(i << 1, l, m, f);
        else insert(i << 1 | 1, m, r, f);
    }

    pair<long long, int> query(long long x) {
        return query(1, lower_bound(xs.begin(), xs.end(), x) - xs.begin(), 0, n);
    }

    pair<long long, int> query(int i, int pos, int l, int r) {
        if (l + 1 == r) return {ST[i](xs[pos]), ST[i].i};

        int m = midpoint(l, r);
        if (pos < m) return max({ST[i](xs[pos]), ST[i].i}, query(i << 1, pos, l, m));
        else return max({ST[i](xs[pos]), ST[i].i}, query(i << 1 | 1, pos, m, r));
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    LiChaoSegmentTree(int n, const vector<long long> &xs) : n(n), xs(xs), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<long long, long long, int>> events(n);
    vector<long long> V;
    for (auto &[t, v, e] : events) {
        cin >> v >> t >> e;

        V.emplace_back(v);
    }
    sort(events.begin(), events.end());
    sort(V.begin(), V.end());
    V.erase(unique(V.begin(), V.end()), V.end());

    int M;
    cin >> M;

    vector<vector<bool>> adj_matrix(6, vector<bool>(6, true));
    while (M--) {
        int a, b;
        cin >> a >> b;

        adj_matrix[a][b] = adj_matrix[b][a] = false;
    }

    vector<LiChaoSegmentTree> lcsts(6, LiChaoSegmentTree(V.size(), V));
    vector<int> prev(n, -1);
    int j = -1;
    long long m = -1e10;
    for (int i = 0; i < n; i++) {
        auto [t, v, e] = events[i];

        int k = -1;
        long long satisfaction = -1e10;
        for (int d = 1; d <= 5; d++)
            if (adj_matrix[d][e]) {
                auto [s, l] = lcsts[d].query(v);
                if (satisfaction < s) {
                    satisfaction = s;
                    k = l;
                }
            }

        LiChaoSegmentTree::Function f(v, 0, i);
        if (satisfaction < v) satisfaction = v;
        else {
            f.c = satisfaction;
            prev[i] = k;
        }
        lcsts[e].insert(f);

        if (m < satisfaction) {
            m = satisfaction;
            j = i;
        }
    }

    vector<int> subseq;
    for (; ~j; j = prev[j]) subseq.emplace_back(j + 1);
    reverse(subseq.begin(), subseq.end());

    cout << m << "\n" << subseq.size() << "\n";
    for (int i : subseq) cout << i << " ";
}
