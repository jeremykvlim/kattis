#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> freq(n, vector<int>(26, 0));
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        for (char c : s) freq[i][c - 'a']++;
    }

    vector<int> dp1(1 << n, 0), dp2(1 << n);
    for (int mask = 1; mask < 1 << n; mask++)
        for (int c = 0; c < 26; c++) {
            int least = INT_MAX;
            for (int i = 0; i < n; i++)
                if ((mask >> i) & 1) least = min(least, freq[i][c]);

            dp1[mask] += least;
        }

    for (int m1 = 3; m1 < 1 << n; m1++) {
        if (popcount((unsigned) m1) == 1) continue;

        dp2[m1] = INT_MAX;
        for (int m2 = m1 ^ (1 << __lg(m1)); m2; m2 = m1 & (m2 - 1))
            dp2[m1] = min(dp2[m1], dp2[m2] + dp2[m1 ^ m2] + dp1[m2] + dp1[m1 ^ m2] - 2 * dp1[m1]);
    }

    cout << dp1.back() + dp2.back() + 1;
}
