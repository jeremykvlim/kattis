#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string S, T;
        cin >> S >> T;

        int n = S.size(), m = T.size();
        vector<vector<bool>> same(m + 1, vector<bool>(m + 1, false));
        vector<vector<int>> repeats(m + 1, vector<int>(m + 1, 0));
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j <= m; j++) {
                int len = j - i;

                auto s = T.substr(i, len), s_rev = s;
                reverse(s_rev.begin(), s_rev.end());
                for (int k = 0; k <= n - len; k++)
                    if (S.substr(k, len) == s || S.substr(k, len) == s_rev) same[i][j] = true;
                for (int k = 0; k <= m - len; k++)
                    if (T.substr(k, len) == s || T.substr(k, len) == s_rev) repeats[i][j] |= 1 << k;
            }

        vector<int> dp(1 << m, m + 1), mask(m);
        dp[0] = 0;
        for (int i = 0; i < 1 << m; i++)
            if (dp[i] <= m) {
                for (int j = 0, k = i; j < m; j++, k &= k >> 1) mask[j] = k;
                for (int j = 0, k = 0; j < m; j++)
                    if (!((i >> j) & 1)) {
                        k = max(k, j);
                        while (k < m && !((i >> k) & 1) && same[j][k + 1] | (mask[k - j] & repeats[j][k + 1])) k++;

                        if (j != k) dp[i + (1 << k) - (1 << j)] = min(dp[i + (1 << k) - (1 << j)], dp[i] + 1);
                    }
            }

        cout << (dp.back() <= m ? to_string(dp.back()) : "impossible") << "\n";
    }
}
