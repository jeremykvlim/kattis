#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    vector<int> choice(n + 1, 0);
    for (int i = 0; i < p; i++) {
        int a, b;
        cin >> a >> b;
      
        choice[max(a, b)] = max(choice[max(a, b)], min(a, b));
    }

    vector<long long> dp(n + 1, 1);
    auto total = 1LL;
    for (int i = 1, j = 0; i <= n; total += dp[i++]) {
        while (j < choice[i]) total -= dp[j++];
        total %= MODULO;
        dp[i] = total;
    }

    cout << (dp.back() < 0 ? dp.back() + MODULO : dp.back());
}
