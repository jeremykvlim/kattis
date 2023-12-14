#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n;
    
    vector<int> costs(n);
    for (auto &c : costs) cin >> c;

    cin >> m;
    vector<int> orders(m);
    for (auto &s : orders) cin >> s;
    m = *max_element(orders.begin(), orders.end());
    vector<long long> dp(m + 1, 0);

    dp[0] = 1;
    for (int i = 0; i < n; i++)
        for (int j = costs[i]; j <= m; j++) dp[j] += dp[j - costs[i]];

    for (int s : orders) {
        if (!dp[s]) cout << "Impossible\n";
        else if (dp[s] > 1) cout << "Ambiguous\n";
        else {
            vector<int> v;
            for (int i = n - 1; i >= 0 && s > 0; i--)
                while (s >= costs[i] && dp[s - costs[i]]) {
                    v.emplace_back(i + 1);
                    s -= costs[i];
                }

            for (int i = v.size() - 1; i >= 0; i--) cout << v[i] << " ";
        }
    }
}
