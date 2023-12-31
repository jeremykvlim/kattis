#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, M;
    cin >> n >> M;

    vector<int> c(n + 1), a(n + 1), dp(n + 1, 0), lis(n + 1, 0), alt(n + 1, 0);
    for (int i = 1; i <= n; i++) cin >> c[i] >> a[i];
    alt[0] = -1;
    int m = 0, last = 0;
    for (int i = 1, prev = 0, len = 0; i <= n; i++) {
        int l = -1, r = m + 1, mid;
        while (l + 1 < r) {
            mid = l + (r - l) / 2;
            if (a[lis[mid]] < a[i] && c[lis[mid]] != c[i]) len = l = mid;
            else if (a[lis[mid]] < a[i] && c[lis[mid]] == c[i]) {
                l = mid;
                len = (alt[mid] != -1 && a[alt[mid]] < a[i]) ? l : l - 1;
            } else r = mid;
        }

        dp[i] = ++len;
        if (len > m) {
            lis[len] = i;
            alt[len] = -1;
            m++;
        } else {
            if (a[lis[len]] >= a[i]) {
                if (c[lis[len]] != c[i]) alt[len] = lis[len];
                lis[len] = i;
            }  else if (c[lis[len]] != c[i] && (alt[len] == -1 || a[alt[len]] > a[i])) alt[len] = i;
        }

        if (prev < len) {
            prev = len;
            last = i;
        }
    }
    cout << m << "\n";

    vector<int> dances(m);
    dances[m - 1] = last;
    for (int i = last - 1, j = i + 1, k = m - 2; i; i--)
        if (dp[i] == dp[j] - 1 && a[i] < a[j] && c[i] != c[j]) j = dances[k--] = i;

    for (int d : dances) cout << d << " ";
}
