#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 4>> exams(n);
    for (auto &[s, p, e, a] : exams) cin >> s >> p >> e >> a;
    
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 1e10));
    dp[0][0] = 0;
    for (int i = 0; i < n; i++) {
        auto [s, p, e, a] = exams[i];
        for (int j = 0; j <= i; j++) {
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + e - s);
            if (dp[i][j] + a <= s) dp[i + 1][j + 1] = min(dp[i + 1][j + 1], dp[i][j] + a - s + p);
        }
    }
    
    for (int j = n; ~j; j--) 
        if (dp[n][j] < 1e10) {
            cout << j;
            exit(0);
        }
}
