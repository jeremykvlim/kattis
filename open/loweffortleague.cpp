#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<pair<T, int>> smawk(int n, int m, auto &&get, auto &&cmp) {
    int lg = __lg(n);
    vector<pair<T, int>> dp(n);
    vector<int> cols(n), offset(lg + 1, 0);
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

    int r;
    cin >> r;

    int n = 1 << r;
    vector<int> s(n);
    for (int &si : s) cin >> si;

    vector<long long> dp(n), temp(n);
    auto dnc = [&](auto &&self, int l, int r) -> vector<int> {
        if (l + 1 == r) return {l};

        int m = l + (r - l) / 2;
        auto left = self(self, l, m), right = self(self, m, r);

        auto update = [&](const vector<int> &v1, const vector<int> &v2) {
            auto get = [&](int row, int col) {
                int d = max(0, s[v1[col]] - s[v2[row]]);
                return dp[v1[col]] + (long long) d * d;
            };

            auto cmp = [&](const auto &p1, const auto &p2) {
                return p1 > p2;
            };
            auto rows = smawk<long long>(v2.size(), v1.size(), get, cmp);
            for (int i = 0; i < v2.size(); i++) temp[v2[i]] = dp[v2[i]] + rows[i].first;
        };
        update(right, left);
        update(left, right);

        vector<int> both;
        merge(left.begin(), left.end(), right.begin(), right.end(), back_inserter(both), [&](int i, int j) { return s[i] != s[j] ? s[i] < s[j] : i < j; });
        for (int i : both) dp[i] = temp[i];
        return both;
    };
    dnc(dnc, 0, n);
    cout << dp[0];
}
