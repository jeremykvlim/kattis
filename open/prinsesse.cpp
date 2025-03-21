#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, s;
    cin >> m >> n >> s;

    vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; i++) dp[i] = min(dp[i - 1] + (i >= s + 1 ? dp[i - s - 1] : 0) + 1, m);

    int l = -1, r = m - 1, mid;
    while (l + 1 < r && n > 0) {
        mid = l + min(n >= s + 1 ? dp[n - s - 1] + 1 : 1, r - l);

        cout << "? " ;
        for (int i = l + 1; i <= mid; i++) cout << i << " ";
        cout << "\n" << flush;

        bool b;
        cin >> b;

        if (b) {
            r = mid;
            n -= s + 1;
        } else {
            l = mid;
            n--;
        }
    }
    cout << "! " << r;
}
