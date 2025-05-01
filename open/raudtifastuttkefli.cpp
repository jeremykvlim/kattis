#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> l(n + 1), D(n + 1);
    for (int i = 1; i <= n; i++) cin >> l[i] >> D[i];

    int t;
    cin >> t;

    vector<int> diff, indices;
    vector<long long> pref_l(n + 1, 0), pref_d(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        pref_l[i] = pref_l[i - 1];
        pref_d[i] = pref_d[i - 1];
        if (l[i] <= k) {
            pref_l[i] += l[i];
            pref_d[i] += D[i];
        } else {
            diff.emplace_back(l[i] - k);
            indices.emplace_back(i);
        }
    }
    int s = 0;
    for (int d : diff) s += d;

    if (!s) {
        auto dopamine = 0LL;
        for (int i = 1; i <= n; i++)
            if (l[i] <= k) {
                if (t >= l[i]) {
                    t -= l[i];
                    dopamine += D[i];
                } else break;
            }
        cout << dopamine;
        exit(0);
    }

    auto dopamine = pref_d[indices[0] - 1];
    vector<long long> dp(s + 1, -1);
    dp[0] = 0;
    for (int i = 0; i < diff.size(); i++) {
        for (int j = s; j >= diff[i]; j--)
            if (dp[j - diff[i]] != -1) dp[j] = max(dp[j], dp[j - diff[i]] + D[indices[i]]);

        auto r = (long long) t - pref_l[indices[i]] - (long long) k * (i + 1);
        if (r < 0) break;
        dopamine = max(dopamine, (i + 1 < diff.size() ? pref_d[indices[i + 1] - 1] : pref_d[n]) + *max_element(dp.begin(), dp.begin() + min(r, (long long) s) + 1));
    }
    cout << dopamine;
}
