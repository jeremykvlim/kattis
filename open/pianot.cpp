#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    unordered_map<char, int> indices;
    for(int i = 0; i < s.size(); i++)
        if (!indices.count(s[i])) indices[s[i]] = indices.size();
    int n = indices.size();

    vector<vector<int>> dist(n, vector<int>(n, 0));
    for (int i = 0; i < s.size() - 1; i++) {
        int u = indices[s[i]], v = indices[s[i + 1]];
        dist[u][v]++;
        dist[v][u]++;
    }

    vector<long long> dp(1 << n, 1e18);
    dp[0] = 0;
    for (int mask = 0; mask < 1 << n; mask++) {
        auto d = dp[mask];
        for (int i = 0; i < n; i++)
            if (!((mask >> i) & 1))
                for (int j = 0; j < n; j++)
                    if ((mask >> j) & 1) d += dist[i][j];

        for (int i = 0; i < n; i++)
            if (!((mask >> i) & 1)) dp[mask | (1 << i)] = min(dp[mask | (1 << i)], d);
    }
    cout << dp.back();
}
