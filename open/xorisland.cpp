#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> hats(n);
    for (int &a : hats) cin >> a;

    vector<bool> dp(1 << n, false);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++) dp[(1 << i) | (1 << j) | (1 << k)] = !(hats[i] ^ hats[j] ^ hats[k]);

    int meetings = n;
    for (int mask = 0; mask < 1 << n; mask++)
        if (!dp[mask]) meetings = min(meetings, n - popcount((unsigned) mask));
        else
            for (int i = 0; i < n; i++) dp[mask | (1 << i)] = true;
    cout << meetings;
}
