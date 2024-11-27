#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int x = 1; x <= n; x++) {
        int k;
        string s;
        cin >> k >> s;

        vector<vector<vector<int>>> diff(k + 1, vector<vector<int>>(k + 1, vector<int>(2, 0)));
        for (int h = 0; h < s.size(); h += k)
            for (int i = 1; i <= k; i++)
                for (int j = 1; j <= k; j++)
                    if (i != j) {
                        diff[i][j][0] += s[h + i - 1] != s[h + j - 1];
                        if (h) diff[i][j][1] += s[h + i - 1 - k] != s[h + j - 1];
                    }

        vector<vector<vector<int>>> dp(1 << k, vector<vector<int>>(k + 1, vector<int>(2, 1e9)));
        dp[1][1][0] = 1;
        for (int mask = 0; mask < (1 << k) - 1; mask++)
            for (int i = 1; i <= k; i++)
                for (int a = 0; a < 2; a++)
                    if (dp[mask][i][a] != 1e9)
                        for (int j = 1; j <= k; j++) {
                            int m = 1 << (j - 1);
                            if (!(mask & m))
                                for (int b = 0; b < 2; b++)
                                    if (a + b < 2) dp[mask + m][j][a + b] = min(dp[mask + m][j][a + b], dp[mask][i][a] + diff[i][j][b]);
                        }

        int mask = (1 << k) - 1;
        for (int i = 2; i <= k; i++)
            for (int a = 0; a < 2; a++)
                for (int b = 0; b < 2; b++)
                    if (a + b < 2) dp[mask][1][a + b] = min(dp[mask][1][a + b], dp[mask][i][a] + diff[i][1][b]);

        cout << "Case #" << x << ": " << dp[mask][1][1] << "\n";
    }
}
