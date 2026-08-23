#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    auto deduped = x;
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());
    for (int &xi : x) xi = lower_bound(deduped.begin(), deduped.end(), xi) - deduped.begin();

    int m = deduped.size(), trades = 0;
    dynamic_bitset<> bs(m);
    vector<int> dp(n * (n + 1) / 2), pref(m + 1, 0), pos(m);
    for (int i = 0; i < n; i++) {
        if (i)
            for (int l = 0; l < i; l++) pref[x[l] + 1] = max(pref[x[l] + 1], dp[i * (i - 1) / 2 + l]);
        for (int v = 1; v <= m; v++) pref[v] = max(pref[v], pref[v - 1]);

        bs.reset();
        int p = 0;
        bs[m - 1 - x[i]] = true;
        pos[x[i]] = p++;
        trades = max(trades, dp[i * (i + 1) / 2 + i] = pref[x[i]] + 1);
        for (int r = i + 1; r < n; r++)
            if (x[i] > x[r]) {
                int a = m - 1 - x[r], b = bs[a] ? a : bs.find_next(a), q = b == m ? p++ : pos[m - 1 - b];
                if (a != b) {
                    if (b < m) bs[b] = false;
                    bs[a] = true;
                    pos[x[r]] = q;
                }
                trades = max(trades, dp[r * (r + 1) / 2 + i] = pref[x[r]] + q + 1);
            }
    }
    cout << trades;
}
