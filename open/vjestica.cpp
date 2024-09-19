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
    for (int i = 1; i < 1 << n; i++)
        for (int j = 0; j < 26; j++) {
            int least = INT_MAX;
            for (int k = 0; k < n; k++)
                if (i & (1 << k)) least = min(least, freq[k][j]);

            dp1[i] += least;
        }

    for (int i = 3; i < 1 << n; i++) {
        if (__builtin_popcount(i) == 1) continue;

        dp2[i] = INT_MAX;
        for (int j = i ^ (1 << __lg(i)); j; j = i & (j - 1))
            dp2[i] = min(dp2[i], dp2[j] + dp2[j ^ i] + dp1[j] + dp1[j ^ i] - 2 * dp1[i]);
    }

    cout << dp1.back() + dp2.back() + 1;
}
