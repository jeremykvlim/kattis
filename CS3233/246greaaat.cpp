#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;
    vector<int> S(n + 1, 1), D(n + 1, 1), order(2 * t + 1);
    for (int i = 1; i <= n; i++) cin >> S[i] >> D[i];

    unordered_map<int, int> pos, neg;
    for (int i = 0; i <= n; i++)
        if (S[i] > 0 && (!pos.count(S[i]) || D[i] < D[pos[S[i]]])) pos[S[i]] = i;
        else if (S[i] < 0 && (!neg.count(S[i]) || D[i] < D[neg[S[i]]])) neg[S[i]] = i;

    vector<long long> dp(2 * t + 1, LLONG_MAX);
    dp[0] = 0;
    for (auto [s, i] : pos)
        for (int j = 0; j + s <= 2 * t; j++)
            if (dp[j] != LLONG_MAX && dp[j] + D[i] < dp[j + s]) {
                dp[j + s] = dp[j] + D[i];
                order[j + s] = i;
            }

    for (auto [s, i] : neg)
        for (int j = 2 * t; j + s >= 0; j--)
            if (dp[j] != LLONG_MAX && dp[j] + D[i] < dp[j + s]) {
                dp[j + s] = dp[j] + D[i];
                order[j + s] = i;
            }

    vector<int> cheers;
    for (int i = t; i; i -= S[order[i]]) cheers.emplace_back(order[i]);
    cout << cheers.size() << "\n";
    for (int d : cheers) cout << d + 1 << " ";
}
