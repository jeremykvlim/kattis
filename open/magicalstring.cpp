#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s;
        int k;
        cin >> s >> k;

        int n = s.size();
        k = min(k, n / 3);
        vector<vector<pair<int, int>>> sweep(n);
        for (int l = 0; l < n; l++) {
            vector<int> count(26, 0);
            for (int r = l; r < min(n, l + 5); r++) {
                count[s[r] - 'a']++;
                if (r - l < 2) continue;
                for (int ch = 0; ch < 26; ch++)
                    if (count[ch] == r - l) {
                        sweep[l].emplace_back(r + 1, ch);
                        break;
                    }
            }
        }

        vector<vector<array<int, 3>>> dp(n + 1, vector<array<int, 3>>(k + 1, {INT_MIN, INT_MIN, INT_MIN}));
        dp[0][0][0] = 0;
        for (int l = 0; l < n; l++)
            for (int i = 0; i <= k; i++) {
                dp[l + 1][i][0] = max({dp[l + 1][i][0], dp[l][i][0], dp[l][i][1]});
                if (i != k)
                    for (auto [r, ch] : sweep[l]) {
                        int turned = dp[l][i][1];
                        if (!l || s[l - 1] != ch + 'a') turned = max({turned, dp[l][i][0], dp[l][i][2]});
                        if (turned >= 0) dp[r][i + 1][(r < n && s[r] == ch + 'a') + 1] = max(dp[r][i + 1][(r < n && s[r] == ch + 'a') + 1], turned + r - l);
                    }
            }

        int turned = 0;
        for (int j = 0; j <= k; j++) turned = max({turned, dp[n][j][0], dp[n][j][1]});
        cout << turned << "\n";
    }
}
