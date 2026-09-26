#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n1, n2, s1, s2;
    cin >> t >> n1 >> n2 >> s1 >> s2;

    int q = t / s2;
    vector<int> capacity(q + 1);
    for (int i = 0; i <= q; i++) capacity[i] = (t - i * s2) / s1;

    vector<int> dp(n2 + 1, INT_MIN);
    dp[0] = 0;
    for (int bags = 1; bags <= n1 + n2; bags++) {
        vector<int> temp(n2 + 1, INT_MIN);
        for (int i = 0; i <= n2; i++)
            for (int j = 0; j <= q && j <= i; j++)
                if (dp[i - j] != INT_MIN) temp[i] = max(temp[i], dp[i - j] + capacity[j]);
        dp = temp;

        if (dp[n2] >= n1) {
            cout << bags;
            exit(0);
        }
    }
}
