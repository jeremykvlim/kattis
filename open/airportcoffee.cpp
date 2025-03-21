#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long l;
    int a, b, t, r, n;
    cin >> l >> a >> b >> t >> r >> n;

    vector<long long> pos(n);
    for (auto &p : pos) cin >> p;
    pos.emplace_back(l);

    vector<double> dp(n + 1, 1e18);
    vector<int> prev(n + 1);
    for (int i = 0; i < n; i++) {
        dp[i] = min(dp[i], (double) pos[i] / a);

        if (dp[i + 1] > dp[i] + (double) (pos[i + 1] - pos[i]) / a) {
            dp[i + 1] = dp[i] + (double) (pos[i + 1] - pos[i]) / a;
            prev[i + 1] = i;
        }

        auto d = pos[i] + a * t + b * r;
        int j = upper_bound(pos.begin(), pos.end(), d) - pos.begin();
        if (j <= n && dp[j] > dp[i] + t + r + (double) (pos[j] - d) / a) {
            dp[j] = dp[i] + t + r + (double) (pos[j] - d) / a;
            prev[j] = i;
        }

        if (i == --j) continue;
        if (dp[j] > dp[i] + t + (double) (pos[j] - pos[i] - a * t) / b) {
            dp[j] = dp[i] + t + (double) (pos[j] - pos[i] - a * t) / b;
            prev[j] = i;
        }
    }

    vector<int> carts;
    int i = n;
    while (i) {
        carts.emplace_back(prev[i]);
        i = prev[i];
    }
    reverse(carts.begin(), carts.end());

    cout << carts.size() << "\n";
    for (int i : carts) cout << i << " ";
}
