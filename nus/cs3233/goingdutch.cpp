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
    for (int mask = 1; mask < 1 << m; mask++) {
        int sum = 0;
        for (int i = 0; i < m; i++)
            if ((mask >> i) & 1) {
                sum += balance[i];
                dp[mask] = max(dp[mask], dp[mask ^ (1 << i)]);
            }
        if (!sum) dp[mask]++;
    }

    cout << m - dp.back();
}
