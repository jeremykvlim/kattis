#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> dp(1e4 + 1);
    while (n--) {
        unsigned s, l, t, c;
        cin >> s >> l >> t >> c;

        if (max(s, l) > 1e4) continue;
        for (int m = 1e4; m >= s + l; m--)
            if (t >= 9.8 * m) dp[m] = max(dp[m], dp[m - s - l] + (t * log1p((double) l / (m - l)) - 9.8 * l) / c);
    }
    cout << (int) round(*max_element(dp.begin(), dp.end()));
}
