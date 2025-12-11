#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> lengths(n);
    for (int &l : lengths) cin >> l;

    vector<long long> pref(n);
    pref[0] = lengths[0];
    for (int i = 1; i < n; i++) pref[i] = pref[i - 1] + lengths[i];

    auto dp = pref;
    vector<int> indices(n, -1);
    for (int i = 0, j = -1; i < n; i++) {
        if (~j) dp[i] = min(dp[i], pref[i] - pref[j]);

        int l = i, r = n, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (dp[i] <= pref[m] - pref[i]) r = m;
            else l = m;
        }
        indices[r] = max(indices[r], i);
        if (i + 1 < n) j = max(j, indices[i + 1]);
    }

    long long suff = lengths[n - 1];
    for (int i = n - 2; ~i; i--) {
        dp[n - 1] = min(dp[n - 1], max(dp[i], suff));
        suff += lengths[i];
    }
    cout << dp[n - 1];
}
