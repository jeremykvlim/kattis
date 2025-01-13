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
    vector<vector<pair<int, int>>> dp(n + 1, vector<pair<int, int>>(n + 1));
    for (int r = 1; r <= n; r++) {
        for (auto &row : dp)
            for (auto &p : row) p = {0, 0};

        for (int i = 1; i <= n; i++) {
            dp[i][i].first = dp[i][i].second = m - abs(x[i]) * r;
            water = max(water, dp[i][i].first);
        }

        for (int l = 2; l <= r; l++)
            for (int i = 1, j = l; j <= n; i++, j++) {
                dp[i][j].first = m + max(dp[i + 1][j].first - abs(x[i + 1] - x[i]) * (r - l + 1), dp[i + 1][j].second - abs(x[j] - x[i]) * (r - l + 1));
                dp[i][j].second = m + max(dp[i][j - 1].second - abs(x[j] - x[j - 1]) * (r - l + 1), dp[i][j - 1].first - abs(x[j] - x[i]) * (r - l + 1));
                water = max(water, max(dp[i][j].first, dp[i][j].second));
            }
    }
    cout << water;
}
