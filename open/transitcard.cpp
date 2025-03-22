#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l;
    cin >> l;

    vector<int> p(l), d(l - 1);
    for (int &pi : p) cin >> pi;
    for (int &di : d) cin >> di;
    d.emplace_back(1e6 + 1);

    int t, n;
    cin >> t >> n;

    vector<int> totals(t + 1, 0);
    for (int i = 1, j = 0, s = 0, days = d[0]; i <= t; i++) {
        totals[i] = s += p[j];
        if (!--days)
            if (j < l - 1) {
                j++;
                days = d[j];
            }
    }

    vector<int> a(n), b(n), dp(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i] >> b[i];

        dp[i] = totals[a[i] - 1];
        for (int j = 0; j < i; j++) dp[i] = min(dp[i], dp[j] + totals[a[i] - b[j] - 1]);
    }

    for (int i = 0; i < n; i++) totals[t] = min(totals[t], dp[i] + totals[t - b[i]]);
    cout << totals[t];
}
