#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> x(n + 1);
    for (int i = 1; i <= n; i++) cin >> x[i];
    sort(x.begin() + 1, x.end());

    int water = 0;
    vector<vector<array<int, 2>>> dp(n + 1, vector<array<int, 2>>(n + 1));
    for (int r = 1; r <= n; r++) {
        for (auto &row : dp) fill(row.begin(), row.end(), array{0, 0});

        for (int i = 1; i <= n; i++) water = max(water, dp[i][i][0] = dp[i][i][1] = m - abs(x[i]) * r);
        for (int l = 2; l <= r; l++)
            for (int i = 1, j = l; j <= n; i++, j++) water = max({water,
                                                                  dp[i][j][0] = m + max(dp[i + 1][j][0] - abs(x[i + 1] - x[i]) * (r - l + 1), dp[i + 1][j][1] - abs(x[j] - x[i]) * (r - l + 1)),
                                                                  dp[i][j][1] = m + max(dp[i][j - 1][1] - abs(x[j] - x[j - 1]) * (r - l + 1), dp[i][j - 1][0] - abs(x[j] - x[i]) * (r - l + 1))});
    }
    cout << water;
}
