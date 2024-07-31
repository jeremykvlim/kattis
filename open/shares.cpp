#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c, n, p;
    cin >> c >> n >> p;

    vector<int> a(n + 1), t(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i] >> t[i];

        t[i] -= a[i];
    }

    vector<pair<int, int>> packs;
    while (p--) {
        int r;
        cin >> r;

        int cost = 0, profit = 0;
        while (r--) {
            int s, q;
            cin >> s >> q;

            cost += a[s] * q;
            profit += t[s] * q;
        }
        if (cost <= c && profit) packs.emplace_back(cost, profit);
    }

    if (packs.empty()) {
        cout << 0;
        exit(0);
    }

    if (packs.size() == 1) {
        cout << packs[0].second;
        exit(0);
    }

    int g = c;
    for (auto [cost, profit] : packs) g = __gcd(g, cost);
    c /= g;

    vector<int> dp(c + 1, 0);
    for (auto [cost, profit] : packs) {
        cost /= g;
        for (int i = c; i >= cost; i--) dp[i] = max(dp[i], dp[i - cost] + profit);
    }

    cout << dp[c];
}
