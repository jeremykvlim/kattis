#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    vector<int> a(n), b(n), s(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i] >> s[i];

    vector<vector<long long>> dp(n + 1, vector<long long>(25001, 0));
    for (int i = n - 1; ~i; i--)
        for (int j = 0; j < 25001; j++) {
            auto best = 0LL;
            if (!b[i]) {
                if (j >= s[i]) best = max(best, a[i] + dp[i][j - s[i]]);
                best = max(best, dp[i + 1][j]);
            } else {
                auto sum = 0LL;
                for (int k = 1; a[i] - (k - 1) * (k - 1) * b[i] > 0; k++) {
                    sum += a[i] - (k - 1) * (k - 1) * b[i];
                    
                    if (k * s[i] <= j) best = max(best, sum + dp[i + 1][j - k * s[i]]);
                    else break;
                }
                best = max(best, dp[i + 1][j]);
            }
            dp[i][j] = best;
        }

    int q;
    cin >> q;
    
    while (q--) {
        int t;
        cin >> t;
        cout << dp[0][t] << "\n";
    }
}
