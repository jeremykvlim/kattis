#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n, m, o;
    cin >> k >> n >> m >> o;

    vector<vector<int>> tolls(n, vector<int>(k, INT_MAX));
    while (m--) {
        int a, b, t;
        cin >> a >> b >> t;

        tolls[a][b % k] = t;
    }

    vector<int> ratio(n);
    for (int i = 0; i < n; i++) ratio[i] = i - i % k + k;

    vector<long long> dp(n);
    while (o--) {
        int a, b;
        cin >> a >> b;

        dp[b] = 0;
        for (int i = b + 1; i < n; i++) dp[i] = INT_MAX;

        for (int i = b - 1; i >= a; i--) {
            dp[i] = INT_MAX;
            for (int j = 0; j < k; j++)
                dp[i] = min(dp[i], dp[ratio[i] + j] + tolls[i][j]);
        }

        cout << (dp[a] < INT_MAX ? dp[a] : -1) << "\n";
    }
}
