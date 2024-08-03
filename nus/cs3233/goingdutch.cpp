#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<int> balance(m);
    for (int i = 0; i < n; i++) {
        int a, b, p;
        cin >> a >> b >> p;

        balance[a] -= p;
        balance[b] += p;
    }
    
    vector<int> dp(1 << m, 0);
    for (int i = 1; i < 1 << m; i++) {
        int sum = 0;
        for (int j = 0; j < m; j++)
            if (i & (1 << j)) {
                sum += balance[j];
                dp[i] = max(dp[i], dp[i ^ (1 << j)]);
            }
        if (!sum) dp[i]++;
    }

    cout << m - dp.back();
}
