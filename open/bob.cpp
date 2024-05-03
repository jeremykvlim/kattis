#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> a(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[i][j];

    auto ways = 0LL;
    vector<long long> f(m + 1), dp(m + 1);
    for (int i = 1; i <= n; i++) {
        stack<int> s;
        for (int j = 1, prev = 0; j <= m; j++) {
            (a[i][j] != a[i - 1][j]) ? f[j] = 1 : f[j]++;

            if (a[i][j] != a[i][j - 1]) {
                prev = j - 1;
                dp[j - 1] = 0;
                while (!s.empty()) s.pop();
            }

            while (!s.empty() && f[s.top()] >= f[j]) s.pop();
            
            int k = s.empty() ? prev : s.top();
            dp[j] = dp[k] + (j - k) * f[j];
            s.emplace(j);
            ways += dp[j];
        }
    }

    cout << ways;
}
