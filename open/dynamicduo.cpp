#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    vector<vector<int>> M(m + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < m; i++) cin >> M[i][m];
    for (int j = 0; j < m; j++) cin >> M[m][j];

    for (int s = 2 * m; s; s--) {
        int l = max(0, s - m), r = min(m, s);
        vector<int> seq(r - l + 1);
        for (int k = 0; k <= r - l; k++) seq[k] = M[s - l - k][l + k];
        auto rev = seq;
        reverse(rev.begin(), rev.end());
        
        auto lis_suff = [&](const auto &a) {
            int n = a.size();
            vector<int> dp(n, 1);
            for (int i = n - 1; ~i; i--)
                for (int j = i + 1; j < n; j++)
                    if (a[j] > a[i]) dp[i] = max(dp[i], dp[j] + 1);

            vector<int> lis(n);
            for (int i = n - 1, longest = 0; ~i; i--) {
                longest = max(longest, dp[i]);
                lis[i] = longest;
            }
            return lis;
        };

        auto lis_up = lis_suff(seq), lis_down = lis_suff(rev);
        for (int i = max(0, s - m - 1), j = s - i - 1; i <= min(m, s - 1); i++, j--)
            if (i < m && j < m) M[i][j] = max(lis_up[j + 1 - l], lis_down[r - j]) + M[i + 1][j + 1];
    }
    cout << M[0][0];
}
