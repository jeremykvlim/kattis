#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    unordered_map<int, int> compress;
    vector<int> t(n);
    for (int i = 0; i < n; i++) {
        cin >> t[i];

        compress[t[i]] = i;
    }

    vector<int> s(p), pref, indices;
    for (int i = 0; i < p; i++) {
        int si, ei;
        cin >> si >> ei;

        s[i] = compress[si];
        pref.resize(s[i] + 1, pref.empty() ? 0 : pref.back());
        indices.resize(s[i], i);
        for (int j = s[i]; j < compress[ei]; j++) pref.emplace_back(pref.back() + t[j + 1] - t[j]);
    }
    pref.resize(n, pref.back());
    indices.resize(n, p);

    vector<int> dp(p + 1, n);
    for (int i = 0; i < p; i++) {
        int l = s[i];
        for (int r = l + 1; r < n; r++) {
            while (l + 1 < n && pref[l + 1] - pref[s[i]] <= t[r] - t[l + 1]) l++;
            dp[indices[r]] = max(dp[indices[r]], dp[i] + r - l);
        }
    }

    cout << dp[p];
}
