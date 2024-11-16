#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    string s;
    cin >> n >> k >> s;

    vector<char> c;
    for (char ch : s) c.emplace_back(ch);
    sort(c.begin(), c.end());
    c.erase(unique(c.begin(), c.end()), c.end());

    vector<int> ctoi('z' + 1, 0), t(n);
    for (int i = 0; i < c.size(); i++) ctoi[c[i]] = i;
    for (int i = 0; i < n; i++) t[i] = ctoi[s[i]];

    vector<vector<int>> clicks(1 << k, vector<int>(k, 0));
    for (int m = 0; m < 1 << k; m++) {
        int prev = -1;
        for (int i = 0; i < n; i++) {
            if (m & (1 << t[i])) continue;
            clicks[m][t[i]] += t[i] != prev;
            prev = t[i];
        }
    }

    vector<int> dp(1 << k, 1e9);
    dp[0] = 0;
    for (int m = 0; m < 1 << k; m++) {
        for (int i = 0; i < k; i++) {
            if (m & (1 << i)) continue;
            dp[m | (1 << i)] = min(dp[m | (1 << i)], dp[m] + clicks[m][i]);
        }
    }

    cout << dp.back();
}
