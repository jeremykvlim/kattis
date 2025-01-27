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
    for (int mask = 0; mask < 1 << k; mask++) {
        int prev = -1;
        for (int i = 0; i < n; i++) {
            if ((mask >> t[i]) & 1) continue;
            clicks[mask][t[i]] += t[i] != prev;
            prev = t[i];
        }
    }

    vector<int> dp(1 << k, 1e9);
    dp[0] = 0;
    for (int mask = 0; mask < 1 << k; mask++)
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1) continue;
            dp[mask | (1 << i)] = min(dp[mask | (1 << i)], dp[mask] + clicks[mask][i]);
        }

    cout << dp.back();
}
