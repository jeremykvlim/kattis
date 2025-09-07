#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> s(n), t(n);
    for (int i = 0; i < n; i++) cin >> s[i] >> t[i];

    vector<pair<int, int>> flights(m);
    for (auto &[src, dest] : flights) cin >> src >> dest;

    double highest = 0;
    vector<double> dp(n);
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        vector<double> temp(n);
        for (auto [src, dest] : flights) {
            auto p = max(min(1., ((i + (dest == n - 1)) - s[dest]) / (double) (t[dest] - s[dest])), 0.);
            temp[dest] = max(dp[src] * (1 - p), temp[dest]);
        }

        dp = temp;
        highest = max(dp.back(), highest);
    }
    cout << fixed << setprecision(6) << highest;
}
