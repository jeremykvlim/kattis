#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AffineFunction {
    T m, c;
    int i;

    AffineFunction(T m = 0, T c = numeric_limits<T>::lowest() / 4, int i = -1) : m(m), c(c), i(i) {}

    T operator()(T x) const {
        return m * x + c;
    }
};

template <typename F, typename E, typename C = less<>>
struct LiChaoSegmentTree {
    int n;
    vector<F> ST;
    vector<bool> used, visited;
    F identity;
    E eval;
    C cmp;

    LiChaoSegmentTree(int n, F identity, E eval) : n(n), ST(2 * n, identity), used(2 * n, false), visited(2 * n, false),
                                                   identity(identity), eval(eval), cmp({}) {}

    void update(F f) {
        apply(1, 0, n, f);
    }

    void apply(int i, int l, int r, F f) {
        visited[i] = true;
        if (!used[i]) {
            used[i] = true;
            ST[i] = f;
            return;
        }

        auto fl = eval(f, l), fr = eval(f, r - 1),
             sl = eval(ST[i], l), sr = eval(ST[i], r - 1);

        bool left = cmp(fl, sl), right = cmp(fr, sr);
        if (!left && !right) return;
        if (!cmp(sl, fl) && !cmp(sr, fr)) {
            ST[i] = f;
            return;
        }

        if (l + 1 == r) {
            if (left) ST[i] = f;
            return;
        }

        int m = midpoint(l, r);
        bool mid = cmp(eval(f, m), eval(ST[i], m));
        if (mid) swap(f, ST[i]);
        if (left != mid) apply(i << 1, l, m, f);
        else apply(i << 1 | 1, m, r, f);
    }

    void range_update(int ql, int qr, const F &f) {
        range_update(1, ql, qr, f, 0, n);
    }

    void range_update(int i, int ql, int qr, const F &f, int l, int r) {
        if (qr <= l || r <= ql) return;
        visited[i] = true;
        if (ql <= l && r <= qr) {
            apply(i, l, r, f);
            return;
        }

        int m = midpoint(l, r);
        range_update(i << 1, ql, qr, f, l, m);
        range_update(i << 1 | 1, ql, qr, f, m, r);
    }

    F query(int p) {
        return query(1, p, 0, n);
    }

    F query(int i, int p, int l, int r) {
        if (!visited[i]) return identity;

        F f = used[i] ? ST[i] : identity;
        if (l + 1 == r) return f;

        int m = midpoint(l, r);
        F g = p < m ? query(i << 1, p, l, m) : query(i << 1 | 1, p, m, r);
        if (cmp(eval(g, p), eval(f, p))) f = g;
        return f;
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<long long, long long, int>> events(n);
    vector<long long> V(n);
    for (int i = 0; i < n; i++) {
        auto &[t, v, e] = events[i];
        cin >> v >> t >> e;
        e--;

        V[i] = v;
    }
    sort(events.begin(), events.end());
    sort(V.begin(), V.end());
    V.erase(unique(V.begin(), V.end()), V.end());

    int M;
    cin >> M;

    vector<vector<bool>> adj_matrix(5, vector<bool>(5, true));
    while (M--) {
        int a, b;
        cin >> a >> b;

        adj_matrix[a - 1][b - 1] = adj_matrix[b - 1][a - 1] = false;
    }

    vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        auto [t, v, e] = events[i];
        pos[i] = lower_bound(V.begin(), V.end(), v) - V.begin();
    }

    auto eval = [&](const auto &f, int p) {
        return f(V[p]);
    };

    vector<LiChaoSegmentTree<AffineFunction<long long>, decltype(eval), greater<>>> lcsts(5, {(int) V.size(), {}, eval});
    vector<int> prev(n, -1);
    int j = -1;
    long long m = -1e10;
    for (int i = 0; i < n; i++) {
        auto [t, v, e] = events[i];

        int k = -1;
        long long satisfaction = -1e10;
        for (int d = 0; d < 5; d++)
            if (adj_matrix[d][e]) {
                auto f = lcsts[d].query(pos[i]);
                auto s = eval(f, pos[i]);
                if (satisfaction < s) {
                    satisfaction = s;
                    k = f.i;
                }
            }

        auto c = 0LL;
        if (satisfaction < v) satisfaction = v;
        else {
            c = satisfaction;
            prev[i] = k;
        }
        lcsts[e].update({v, c, i});

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