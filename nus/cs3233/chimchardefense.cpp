#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> h(n), d(n), c(n);
    for (int &hi : h) cin >> hi;
    for (int &di : d) cin >> di;
    for (int &ci : c) cin >> ci;

    int m = *max_element(h.begin(), h.end()) + *max_element(d.begin(), d.end());
    vector<int> dp(m + 1, -1e9);
    dp[0] = 0;
    for (int i = n - 1; ~i; i--)
        for (int j = m; ~j; j--) {
            dp[j] += min(h[i], j);
            if (j >= d[i]) dp[j] = max(dp[j], dp[j - d[i]] + min(h[i], j) - c[i]);
        }
    cout << *max_element(dp.begin(), dp.end());
}
