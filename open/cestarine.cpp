#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> entrance(n), exit(n);
    for (int i = 0; i < n; i++) cin >> entrance[i] >> exit[i];
    sort(entrance.begin(), entrance.end());
    sort(exit.begin(), exit.end());

    auto largest = (long long) max(entrance.back(), exit.back()) * n;
    vector<int> p(3);
    vector<long long> dp(n + 1, largest);
    dp[n] = 0;
    for (int i = n - 1; ~i; i--)
        for (int j = 1; j <= 3 && i + j <= n; j++) {
            iota(p.begin(), p.begin() + j, 0);
            do {
                auto cost = 0LL;
                for (int k = 0; k < j; k++) cost += entrance[i + k] == exit[i + p[k]] ? largest : abs(entrance[i + k] - exit[i + p[k]]);
                dp[i] = min(dp[i], cost + dp[i + j]);
            } while (next_permutation(p.begin(), p.begin() + j));
        }

    cout << dp[0];
}
