#include <bits/stdc++.h>
using namespace std;

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

            for (int x = 0, y = 0; x <= m; x++) {
                y = max(y, x);
                for (; y < min(2 * x, m) && (i ? dp[i - 1][y] : m) < (i < rounds ? dp[i][2 * x - y] : m); y++);
                temp[i][x] = value(x, y);
                if (y > x) temp[i][x] = max(temp[i][x], value(x, y - 1));
            }
        }
        dp = temp;
    }
    cout << dp[k][d];
}
