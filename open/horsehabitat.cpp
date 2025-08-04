#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c, q;
    cin >> r >> c >> q;

    vector<string> grid(r);
    for (auto &row : grid) cin >> row;

    vector<int> height(c);
    vector<vector<int>> dp(r + 1, vector<int>(c + 1));
    for (int i = 0; i < r; i++) {
        stack<pair<int, int>> s;
        s.emplace(-1, 0);
        for (int j = 0; j < c; j++) {
            if (grid[i][j] != '#') height[j]++;
            else height[j] = 0;

            while (height[j] < s.top().first) {
                auto [hk, k] = s.top();
                s.pop();
                auto [hl, l] = s.top();
                dp[hk][j - l]++;
                dp[max(height[j], hl)][j - l]--;
            }
            s.emplace(height[j], j + 1);
        }

        while (s.top().second) {
            auto [hk, k] = s.top();
            s.pop();
            auto [hl, l] = s.top();
            dp[hk][c - l]++;
            dp[max(0, hl)][c - l]--;
        }
    }

    for (int j = 1; j <= c; j++)
        for (int i = r - 1; i; i--) dp[i][j] += dp[i + 1][j];

    for (int i = 1; i <= r; i++) {
        int count = dp[i][c];
        for (int j = c; j; j--) count += exchange(dp[i][j - 1], dp[i][j - 1] + dp[i][j] + count);
    }

    while (q--) {
        int h, w;
        cin >> h >> w;
        cout << dp[h][w] << "\n";
    }
}
