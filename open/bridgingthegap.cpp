#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> t(n);
    for(int &ti : t) cin >> ti;
    sort(t.rbegin(), t.rend());

    vector<long long> suff(n + 1, 0);
    for (int i = n; i; i--) suff[i - 1] = suff[i] + t[i - 1];

    int crosses = (n + c - 1) / c;
    auto least = LLONG_MAX;
    vector<vector<long long>> dp(n + 1, vector<long long>(2 * crosses + 1, 1e18));
    dp[0][crosses] = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= 2 * crosses; j++) {
            int walkers = min(n - i, c);

            for (int k = 2; k <= walkers && j + k - 1 <= 2 * crosses; k++)
                dp[i][j + k - 1] = min(dp[i][j + k - 1], dp[i][j] + t[n - k] + suff[n - k]);

            if (j) dp[i + walkers][j - 1] = min(dp[i + walkers][j - 1], dp[i][j] + t[i]);

            for (int k = 1; k < walkers && j + k - 1 <= crosses; k++)
                dp[i + walkers - k][j + k - 1] = min(dp[i + walkers - k][j + k - 1], dp[i][j] + t[i] + suff[n - k]);

            if (j >= crosses && n - i <= c) least = min(least, dp[i][j] + t[i]);
        }

    cout << least;
}
