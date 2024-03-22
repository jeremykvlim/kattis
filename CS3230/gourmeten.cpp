#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<int> dp(m + 1, 0), t(n);
    dp[0] = 1;
    for (int &ti : t) cin >> ti;

    for (int i = 1; i <= m; i++)
        for (int ti : t)
            if (ti <= i) dp[i] += dp[i - ti];

    cout << dp[m];
}
