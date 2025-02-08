#include <bits/stdc++.h>
using namespace std;

struct LiChaoSegmentTree {
    struct Function {
        __int128 m, c;
        int i;

        Function(__int128 slope = 0, __int128 y_intercept = -1e20, int i = -1) : m(slope), c(y_intercept), i(i) {}

        __int128 operator()(long long x) {
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
        int m = midpoint(l, r);
        bool left = f(xs[l]) > ST[i](xs[l]), mid = f(xs[m]) > ST[i](xs[m]);
        if (mid) swap(ST[i], f);

        if (l + 1 == r) return;
        if (left != mid) insert(i << 1, l, m, f);
        else insert(i << 1 | 1, m, r, f);
    }

    pair<__int128, int> query(long long x) {
        return query(1, lower_bound(xs.begin(), xs.end(), x) - xs.begin(), 0, n);
    }

    pair<__int128, int> query(int i, int pos, int l, int r) {
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

        adj_matrix[a][b] = false;
        adj_matrix[b][a] = false;
    }

    vector<LiChaoSegmentTree> lcsts(6, LiChaoSegmentTree(V.size(), V));
    vector<int> prev(n, -1);
    int j = -1;
    __int128 m = -1e20;
    for (int i = 0; i < n; i++) {
        auto [t, v, e] = events[i];

        int k = -1;
        __int128 satisfaction = -1e20;
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
    while (j != -1) {
        subseq.emplace_back(j + 1);
        j = prev[j];
    }
    reverse(subseq.begin(), subseq.end());

    cout << (long long) m << "\n" << subseq.size() << "\n";
    for (int i : subseq) cout << i << " ";
}
