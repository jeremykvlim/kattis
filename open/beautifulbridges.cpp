#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AffineFunction {
    T m, c;

    AffineFunction(T m = 0, T c = numeric_limits<T>::max() / 4) : m(m), c(c) {}

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

    int n, h, alpha, beta;
    cin >> n >> h >> alpha >> beta;

    vector<int> x(n), verti(n);
    for (int i = 0; i < n; i++) {
        int y;
        cin >> x[i] >> y;

        verti[i] = h - y;
    }

    auto arch_limit = [&](const auto &X, const auto &V) {
        auto eval = [&](int l, int r) -> long long {
            if (l < 0) return INT_MAX / 4;
            if (l > r) return -l - 1;

            int d = X[r] - X[l];
            return 2 * (d + V[l]) + sqrt(8LL * d * V[l]);
        };

        LiChaoSegmentTree<int, decltype(eval)> lcst(n, -1, eval);
        vector<int> limit(n);
        for (int i = 0; i < n; i++) {
            lcst.update(i);
            limit[i] = eval(lcst.query(i), i);
        }
        return limit;
    };
    auto limit = arch_limit(x, verti);

    vector<int> x_rev(n), verti_rev(n);
    for (int i = 0; i < n; i++) {
        x_rev[i] = -x[n - 1 - i];
        verti_rev[i] = verti[n - 1 - i];
    }
    auto limit_rev = arch_limit(x_rev, verti_rev);
    reverse(limit_rev.begin(), limit_rev.end());

    vector<int> pos(n), pos_rev(n);
    for (int i = 0; i < n; i++) {
        pos[i] = lower_bound(x.begin(), x.end(), x[i] - limit[i]) - x.begin();
        pos_rev[i] = upper_bound(x.begin(), x.end(), x[i] + limit_rev[i]) - x.begin();
    }

    vector<long long> dp(n, 1e18);
    dp[0] = alpha * verti[0];
    auto dnc = [&](auto &&self, int l, int r) -> void {
        if (l + 1 == r) return;

        int m = l + (r - l) / 2;
        self(self, l, m);

        vector<int> order(r - m);
        iota(order.begin(), order.end(), m);
        sort(order.begin(), order.end(), [&](int i, int j) { return pos[i] > pos[j]; });

        auto eval = [&](const AffineFunction<long long> &f, int p) {
            return f(x[m + p]);
        };

        LiChaoSegmentTree<AffineFunction<long long>, decltype(eval)> lcst(r - m, {}, eval);
        for (int j = m - 1; int i : order) {
            for (; j >= l && j >= pos[i]; j--)
                if (dp[j] != 1e18) lcst.range_update(0, pos_rev[j] - m, {-2LL * beta * x[j], dp[j] + (long long) beta * x[j] * x[j]});
            dp[i] = min(dp[i], alpha * verti[i] + (long long) beta * x[i] * x[i] + eval(lcst.query(i - m), i - m));
        }
        self(self, m, r);
    };
    dnc(dnc, 0, n);

    if (dp[n - 1] == 1e18) cout << "impossible";
    else cout << dp[n - 1];
}