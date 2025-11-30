#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AffineFunction {
    T m, c;
    int xr;

    AffineFunction(T m = 0, T c = 1e18, int xr = -1) : m(m), c(c), xr(xr) {}

    T operator()(T x) {
        return x > xr ? 1e18 : m * x + c;
    }
};

template <typename T>
struct LiChaoSegmentTree {
    int n;
    vector<AffineFunction<T>> ST;

    void insert(AffineFunction<T> f) {
        insert(1, 0, n, f);
    }

    void insert(int i, int l, int r, AffineFunction<T> f) {
        bool left = f(l) < ST[i](l);
        if (l + 1 == r) {
            if (left) ST[i] = f;
            return;
        }

        int m = midpoint(l, r);
        bool mid = f(m) < ST[i](m);
        if (mid || f.m == ST[i].m && (f.xr > ST[i].xr || f.c < ST[i].c)) swap(f, ST[i]);

        if (left != mid) insert(i << 1, l, m, f);
        else insert(i << 1 | 1, m, r, f);
    }

    T query(int x) {
        return query(1, x, 0, n);
    }

    T query(int i, int x, int l, int r) {
        if (l + 1 == r) return ST[i](x);

        int m = midpoint(l, r);
        if (x < m) return min(ST[i](x), query(i << 1, x, l, m));
        else return min(ST[i](x), query(i << 1 | 1, x, m, r));
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    LiChaoSegmentTree(int n) : n(n), ST(2 * n) {}
};

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
    SparseTable(vector<T> v, function<T(T, T)> func) : f(move(func)) {
        if (v.empty()) return;
        int n = __lg(v.size()) + 1;
        ST.resize(n);
        ST.front() = v;
        for (int i = 1; i < n; i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j <= v.size() - (1 << i); j++)
                ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    T range_query(int l, int r) {
        int i = __lg(r - l);
        return f(ST[i][l], ST[i][r - (1 << i)]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> X(n), pos(n, 0), neg(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> X[i];

        int diff = !i ? X[i] : X[i] - X[i - 1];
        if (diff > 0) pos[i] = __lg(diff) + 2;
        else if (diff < 0) {
            int lg = __lg(-diff);
            if ((1 << lg) < -diff) lg++;
            neg[i] = lg + 1;
        }
    }

    SparseTable<int> st_x(X, [](int x, int y) { return max(x, y); }), st_pos(pos, [](int x, int y) { return max(x, y); }), st_neg(neg, [](int x, int y) { return max(x, y); });
    vector<vector<tuple<int, long long, int>>> funcs(n + 1);
    vector<long long> dp(n + 1, 1e18);
    dp[0] = 0;
    vector<LiChaoSegmentTree<long long>> lcsts(31, n + 1);
    for (int xl = 0; xl <= n; xl++) {
        if (xl) {
            for (auto [slope, y_intercept, xr] : funcs[xl]) lcsts[slope].insert({slope, y_intercept, xr});
            for (int slope = 0; slope <= 30; slope++) dp[xl] = min(dp[xl], lcsts[slope].query(xl));
        }

        if (xl < n) {
            for (int xr = xl; xr < n;) {
                int l = xr, r = n, m, slope = __lg(st_x.range_query(xl, xr + 1)) + 1;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;

                    if (__lg(st_x.range_query(xl, m + 1)) < slope) l = m;
                    else r = m;
                }
                xr = r;
                funcs[xl + 1].emplace_back(slope, dp[xl] + c - (long long) slope * xl, xr);
            }

            int slope = pos[xl] || neg[xl] ? pos[xl] ^ neg[xl] : 0;
            for (int xr = xl; xr < n && slope <= 30;) {
                int l = xr, r = n, m;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;

                    if (max(st_pos.range_query(xl, m + 1), st_neg.range_query(xl, m + 1)) == slope) l = m;
                    else r = m;
                }
                xr = r;
                funcs[xl + 1].emplace_back(slope, dp[xl] + c - (long long) slope * xl, xr);
                if (xr == n) break;
                slope = max(st_pos.range_query(xl, xr + 1), st_neg.range_query(xl, xr + 1));
            }
        }
    }
    cout << dp[n];
}
