#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();
    vector<int> dp = {0, n + 1};
    for (int i = 0; i < n / 2; i++) {
        if (s[i] != s[n - i - 1]) dp = {dp[1], min(dp[0], dp[1]) + 1};
        else dp = {min(dp[0], dp[1] + 1), dp[1] + 1};
    }
    
    cout << min(dp[0], dp[1]);
}
