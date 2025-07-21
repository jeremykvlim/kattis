#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int total = 0;
    vector<int> count(k, 0);
    while (n--) {
        int w;
        cin >> w;

        count[w % k]++;
        total += w;
    }

    for (int i = 1, j = k - 1; i < j; i++, j--) {
        int c = min(count[i], count[j]);
        count[0] += c;
        count[i] -= c;
        count[j] -= c;
    }

    if (!(k & 1)) {
        count[0] += count[k / 2] / 2;
        count[k / 2] &= 1;
    }

    int m = 1;
    vector<int> step(k);
    for (int i = 1; i < k; i++) {
        step[i] = m;
        m *= count[i] + 1;
    }

    vector<vector<int>> dp(m, vector<int>(k, 0));
    for (int mask = 1; mask < m; mask++)
        for (int i = 1; i < k; i++)
            if (count[i] && mask / step[i] % (count[i] + 1))
                for (int r = 0; r < k; r++) dp[mask][r] = max(dp[mask][r], dp[mask - step[i]][(r + i) % k] + !r);
    cout << total / k - count[0] - dp.back()[0];
}
