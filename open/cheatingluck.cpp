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

    int d, g, n, k;
    cin >> d >> g >> n >> k;
    int m = d + g;
    
    vector<vector<int>> dp(k + 1, vector<int>(m + 1, 0));
    iota(dp[0].begin(), dp[0].end(), 0);
    for (int rounds = 1; rounds <= n; rounds++) {
        vector<vector<int>> temp(min(rounds, k) + 1, vector<int>(m + 1, 0));
        for (int i = 0; i <= min(rounds, k); i++) {
            auto value = [&](int x, int y) {
                return min(i ? dp[i - 1][y] : m, i < rounds ? dp[i][2 * x - y] : m);
            };

            auto get = [&](int x, int y) -> int {
                int z = min(2 * x, m);
                if (x > y) return -1e9 - x + y;
                if (y > z) return -1e9 - y + z;
                return value(x, y) * (m + 1) - y;
            };

            auto cmp = [&](const auto &p1, const auto &p2) {
                return p1.first < p2.first;
            };

            auto rows = smawk<int>(m + 1, m + 1, get, cmp);
            for (int j = 0; j <= m; j++) temp[i][j] = value(j, rows[j].second);
        }
        dp = temp;
    }
    cout << dp[k][d];
}
