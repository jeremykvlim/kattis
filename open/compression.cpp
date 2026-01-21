#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> x(n), diff(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i];

        diff[i] = !i ? x[i] : x[i] - x[i - 1];
    }

    vector<vector<int>> r1(31, vector<int>(n + 1, n)), r2(31, vector<int>(n + 1, n));
    for (int s = 0; s <= 30; s++) {
        r1[s][n] = r2[s][n] = n;
        for (int i = n - 1, l = s ? -(1 << (s - 1)) : 0, r = s ? (1 << (s - 1)) - 1 : 0; ~i; i--) {
            r1[s][i] = x[i] < 1 << s ? r1[s][i + 1] : i;
            r2[s][i] = l <= diff[i] && diff[i] <= r ? r2[s][i + 1] : i;
        }
    }

    vector<priority_queue<pair<long long, int>>> pqs(31);
    vector<long long> dp(n + 1, 1e18);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        if (dp[i] != 1e18)
            for (int s = 0, l = i - 1; s <= 30 && l < n - 1; s++) {
                int r = max(r1[s][i] - 1, r2[s][i] - 1);
                if (l < r) {
                    l = r;
                    pqs[s].emplace(-dp[i] - c + (long long) s * i, r + 1);
                }
            }

        for (int s = 0; s <= 30; s++) {
            while (!pqs[s].empty() && pqs[s].top().second < i + 1) pqs[s].pop();
            if (!pqs[s].empty()) dp[i + 1] = min(dp[i + 1], -pqs[s].top().first + (long long) s * (i + 1));
        }
    }
    cout << dp[n];
}
