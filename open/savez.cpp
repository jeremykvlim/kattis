#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    int tps = 0;
    vector<int> dp(n + 1, 1);
    unordered_map<string, int> indices;
    for (int i = 0; i < n; i++) {
        string s, s1, s2;
        cin >> s;

        for (int j = 0; j < s.size(); j++) {
            s1 += s[j];
            s2 = s[s.size() - j - 1] + s2;
            if (s1 == s2 && indices.count(s1)) dp[i] = max(dp[i], dp[indices[s1]] + 1);
        }
        indices[s] = i;
        tps = max(tps, dp[i]);
    }

    cout << tps;
}
