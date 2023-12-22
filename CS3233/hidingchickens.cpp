#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double x, y;
    int n;
    cin >> x >> y >> n;

    vector<pair<double, double>> hiding(n);
    for (auto &p : hiding) cin >> p.first >> p.second;

    vector<double> dp(1 << n, DBL_MAX);
    dp[0] = 0;
    for (int mask = 1; mask < 1 << n; mask++)
        for (int i = 0; i < n; i++) {
            if (!(mask & 1 << i)) continue;
            for (int j = 0; j <= i; j++) {
                if (!(mask & 1 << j)) continue;

                int submask = 1 << i | 1 << j;
                double dist = hypot(hiding[i].first - hiding[j].first, hiding[i].second - hiding[j].second);
                dist += (submask == mask) ? min(hypot(hiding[i].first - x, hiding[i].second - y), hypot(hiding[j].first - x, hiding[j].second - y))
                                          : hypot(hiding[i].first - x, hiding[i].second - y) + hypot(hiding[j].first - x, hiding[j].second - y);

                dp[mask] = min(dp[mask], dp[mask ^ submask] + dist);
            }
        }

    cout << fixed << setprecision(6) << dp[(1 << n) - 1];
}
