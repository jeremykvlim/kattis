#include <bits/stdc++.h>
using namespace std;

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

        auto fl = eval(f, l), fr = eval(f, r - 1), sl = eval(ST[i], l), sr = eval(ST[i], r - 1);
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

    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

    vector<long long> pref(n, 0);
    for (int i = 1; i < n; i++) pref[i] = pref[i - 1] + min(a[i], b[i - 1]);

    vector<int> xs(b.begin() + 1, b.end());
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());

    auto eval = [&](const pair<int, long long> &f, int p) {
        return min(xs[p], f.first) + f.second;
    };

    LiChaoSegmentTree<pair<int, long long>, decltype(eval), greater<>> lcst(xs.size(), {0, LLONG_MIN / 4}, eval);
    vector<long long> dp(n + 1, -1e18);
    dp[0] = 0;
    for (int j = 2; j <= n; j++) {
        int i = j - 2;
        if (dp[i] != -1e18) lcst.update({a[i], dp[i] - pref[i]});
        int p = lower_bound(xs.begin(), xs.end(), b[j - 1]) - xs.begin();
        dp[j] = pref[j - 1] + eval(lcst.query(p), p);
    }
    cout << dp[n];
}
