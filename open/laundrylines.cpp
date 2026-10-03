#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> w(n);
    for (int &wi : w) cin >> wi;

    int m = 1 << n;
    vector<bool> parity(m, false);
    for (int mask = 1; mask < m; mask++) {
        int i = countr_zero((unsigned) mask);
        parity[mask] = parity[mask ^ (1 << i)] ^ (w[i] & 1);
    }

    int l = -1, r = *max_element(w.begin(), w.end()), mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        vector<bitset<1001>> dp(m), valid(2);
        dp[0][500] = true;
        for (int p = 0; p < 2; p++)
            for (int q = -500; q <= 500; q++)
                if (abs(p + 2 * q) <= mid) valid[p][q + 500] = true;

        for (int mask = 0; mask < m; mask++) {
            bool p = parity[mask];
            dp[mask] &= valid[p];
            if (dp[mask].any())
                for (int i = 0; i < n; i++)
                    if (!((mask >> i) & 1)) {
                        dp[mask | (1 << i)] |= dp[mask] << ((w[i] + p - (p ^ (w[i] & 1))) / 2);
                        dp[mask | (1 << i)] |= dp[mask] >> ((w[i] + (p ^ (w[i] & 1)) - p) / 2);
                    }
        }

        if ((dp[m - 1] & valid[parity[m - 1]]).any()) r = mid;
        else l = mid;
    }

    cout << r;
}