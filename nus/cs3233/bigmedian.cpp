#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto deduped = a;
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

    vector<int> dp(n + 1), pref(n + 1);
    int l = -1, r = deduped.size(), m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        pref[0] = 0;
        for (int i = 1; i <= n; i++) pref[i] = pref[i - 1] + (a[i - 1] >= deduped[m] ? 1 : -1);

        fill(dp.begin(), dp.end(), -1e9);
        dp[0] = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 0; j < i; j++)
                if ((i - j) & 1 && pref[i] > pref[j]) dp[i] = max(dp[i], dp[j] + 1);

        if (dp[n] >= k) l = m;
        else r = m;
    }
    cout << deduped[l];
}
