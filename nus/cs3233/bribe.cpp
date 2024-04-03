#include <bits/stdc++.h>
using namespace std;

double convert(int mask, int need, int money, vector<int> &b, vector<double> &p, vector<vector<double>> &dp) {
    if (dp[mask][need] > -1) return dp[mask][need];
    if (!need) return dp[mask][need] = 1;
    if (need > __builtin_popcount(mask)) return dp[mask][need] = 0;

    double prob = 0;
    for (int i = 0; i < b.size(); i++) {
        if (!(mask & 1 << i) || b[i] > money) continue;
        auto success = p[i] * convert(mask ^ 1 << i, need - 1, money - b[i], b, p, dp), failure = (1 - p[i]) * convert(mask ^ 1 << i, need, money - b[i], b, p, dp);
        prob = max(prob, success + failure);
    }

    return dp[mask][need] = prob;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, c, m;
        cin >> n >> c >> m;

        vector<int> b(n);
        vector<double> p(n);
        for (int i = 0; i < n; i++) {
            cin >> b[i] >> p[i];
            
            p[i] /= 100;
        }

        vector<vector<double>> dp(1 << n, vector<double>(c + 1, -2));
        cout << fixed << setprecision(6) << convert((1 << n) - 1, c, m, b, p, dp) << "\n";
    }
}
