#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n;
    vector<int> c(n);
    for (int i = 0; i < n; i++) cin >> c[i];

    cin >> m;
    vector<int> o(m);
    for (int i = 0; i < m; i++) cin >> o[i];
    m = *max_element(o.begin(), o.end());
    vector<long long> dp(m + 1, 0);

    dp[0] = 1;
    for (int i = 0; i < n; i++)
        for (int j = c[i]; j <= m; j++)
            dp[j] += dp[j - c[i]];

    for (int s : o) {
        if (!dp[s]) cout << "Impossible\n";
        else if (dp[s] > 1) cout << "Ambiguous\n";
        else {
            vector<int> v;
            for (int i = n - 1; i >= 0 && s > 0; i--)
                while (s >= c[i] && dp[s - c[i]]) {
                    v.push_back(i + 1);
                    s -= c[i];
                }

            for (int i = v.size() - 1; i >= 0; i--) cout << v[i] << " ";
            cout << endl;
        }
    }
}
