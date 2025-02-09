#include <bits/stdc++.h>
using namespace std;

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

    SparseTable<int> st_X(X, [](int x, int y) {return max(x, y);}), st_pos(pos, [](int x, int y) {return max(x, y);}), st_neg(neg, [](int x, int y) {return max(x, y);});
    vector<vector<pair<int, long long>>> L(n + 1), R(n + 1);
    vector<long long> dp(n + 1, 1e18);
    dp[0] = 0;
    auto add_func = [&](int xl, int xr, int slope, long long y_intercept) {
        L[xl].emplace_back(slope, y_intercept);
        if (xr < n) R[xr + 1].emplace_back(slope, y_intercept);
    };

    vector<multiset<long long>> funcs(31);
    for (int xl = 0; xl <= n; xl++) {
        if (xl) {
            for (auto [slope, y_intercept] : L[xl]) funcs[slope].emplace(y_intercept);
            for (auto [slope, y_intercept] : R[xl]) {
                auto it = funcs[slope].find(y_intercept);
                if (it != funcs[slope].end()) funcs[slope].erase(it);
            }

            for (int slope = 0; slope <= 30; slope++)
                if (!funcs[slope].empty()) dp[xl] = min(dp[xl], *funcs[slope].begin() + (long long) slope * xl);
        }

        if (xl < n) {
            for (int xr = xl; xr < n;) {
                int l = xr, r = n, m, slope = __lg(st_X.range_query(xl, xr + 1)) + 1;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;

                    if (__lg(st_X.range_query(xl, m + 1)) < slope) l = m;
                    else r = m;
                }
                xr = l + 1;
                if (xl < xr) add_func(xl + 1, xr, slope, dp[xl] + c - (long long) slope * xl);
            }

            int slope = pos[xl] || neg[xl] ? pos[xl] ^ neg[xl] : 0;
            if (slope > 30) continue;

            for (int xr = xl; xr < n && slope <= 30;) {
                int l = xr, r = n, m;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;

                    if (max(st_pos.range_query(xl, m + 1), st_neg.range_query(xl, m + 1)) == slope) l = m;
                    else r = m;
                }
                xr = l + 1;
                add_func(xl + 1, xr, slope, dp[xl] + c - (long long) slope * xl);
                if (xr >= n) break;
                slope = max(st_pos.range_query(xl, xr + 1), st_neg.range_query(xl, xr + 1));
            }
        }
    }
    cout << dp[n];
}
