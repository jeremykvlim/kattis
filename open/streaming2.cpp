#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    string s;
    cin >> n >> k >> s;

    vector<vector<array<int, 2>>> dp(n + 1, vector<array<int, 2>>(2, {2 * n, 0}));
    dp[0][0] = dp[0][1] = {0, 0};
    int buy = 2 * n;
    for (int i = 0; i <= n; i++) {
        if (i == n) {
            buy = min({buy, dp[n][0][0], dp[n][1][0]});
            break;
        }

        auto parse = [&](bool j) {
            auto update = [&](int i, int count, int date) {
                if (dp[i][j][0] > count) dp[i][j] = {count, date};
                else if (dp[i][j][0] == count) dp[i][j][1] = max(dp[i][j][1], date);
            };

            if (dp[i][j][1] > 1) update(i + 1, dp[i][j][0], dp[i][j][1] - 1);

            if (dp[i][!j][1] > 1) {
                if (i + dp[i][!j][1] - 1 >= n) buy = min(buy, dp[i][!j][0] + 1);
                else update(i + dp[i][!j][1] - 1, dp[i][!j][0] + 1, k + 2 - dp[i][!j][1]);
            }
            update(i + 1, dp[i][0][0] + 1, k);
            update(i + 1, dp[i][1][0] + 1, k);
        };

        if (s[i] == '1' || s[i] == 'B') parse(false);
        if (s[i] == '2' || s[i] == 'B') parse(true);
    }
    cout << buy;
}
