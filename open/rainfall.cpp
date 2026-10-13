#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<pair<T, int>> smawk(int n, int m, auto &&get, auto &&cmp) {
    int lg = __lg(n);
    vector<pair<T, int>> dp(n);
    static vector<int> cols, offset;
    if (cols.size() < n) cols.resize(n);
    if (offset.size() < lg + 1) offset.resize(lg + 1);
    offset[0] = 0;
    for (int b = 0; b < lg; b++) {
        int size = 0;
        auto push = [&](int col, int limit = 0) {
            int temp = size;
            for (; size > limit; size--) {
                int row = (size << (b + 1)) - 1;
                pair<T, int> p{get(row, col), col};
                if (!cmp(dp[row], p)) break;
                dp[row] = p;
            }
            if (size == n >> (b + 1)) return;

            if (size == temp) {
                int row = ((size + 1) << (b + 1)) - 1;
                dp[row] = {get(row, col), col};
            }
            cols[offset[b] + size++] = col;
        };

        if (!b)
            for (int col = 0; col < m; col++) push(col);
        else
            for (int i = offset[b - 1]; i < offset[b]; i++) push(cols[i], (i - offset[b - 1]) >> 1);
        offset[b + 1] = offset[b] + size;
    }

    for (int b = lg; b; b--)
        for (int row = (1 << b) - 1, i = offset[b - 1]; row < n; row += 2 << b) {
            int stop = row + (1 << b) < n ? dp[row + (1 << b)].second : -1, col = cols[i];
            dp[row] = {get(row, col), col};
            if (col == stop) continue;
            for (i++; i < offset[b]; i++) {
                col = cols[i];
                pair<T, int> p{get(row, col), col};
                if (cmp(dp[row], p)) dp[row] = p;
                if (col == stop) break;
            }
        }

    for (int row = 0, col = 0; row < n; row += 2) {
        int stop = row + 1 < n ? dp[row + 1].second : -1;
        dp[row] = {get(row, col), col};
        if (col == stop) continue;
        for (col++; col < m; col++) {
            pair<T, int> p{get(row, col), col};
            if (cmp(dp[row], p)) dp[row] = p;
            if (col == stop) break;
        }
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    double c, d;
    cin >> T >> c >> d;

    vector<int> r(T);
    for (int &ri : r) cin >> ri;

    c *= 3600 * d * d;
    auto wet = 1e20;
    vector<int> dp(T + 1, 0), pref(T + 1, 0);
    for (int _ = 0; _ < 2; _++) {
        for (int i = 0; i < T; i++) pref[i + 1] = pref[i] + r[i];

        auto get = [&](int row, int col) {
            if (row < col) return 1e20;
            return c / (row + 1 - col) - pref[col];
        };

        auto cmp = [&](const auto &p1, const auto &p2) {
            return p1.first > p2.first;
        };

        auto rows = smawk<double>(T, T, get, cmp);
        for (int i = 1; i <= T; i++) dp[i] = rows[i - 1].second;

        for (int i = 0; i < T; i++)
            for (int j = dp[i]; j <= dp[i + 1]; j++) {
                auto k = clamp(r[i] ? j + sqrt(c / r[i]) : i + 1, (double) i, (double) i + 1);
                wet = min(wet, pref[i] - pref[j] + r[i] * (k - i) + c / (k - j));
            }
        reverse(r.begin(), r.end());
    }
    cout << fixed << setprecision(6) << wet;
}