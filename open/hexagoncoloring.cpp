#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> a(n);
    for (int i = 0; i < n; i++) {
        a[i].resize(n - (i & 1));
        for (int &aij : a[i]) cin >> aij;
    }

    int all = 1 << (2 * n);
    vector<vector<vector<int>>> states(2);
    vector<vector<int>> masks(2, vector<int>(all));

    auto boundary = [&](int mask, int b) -> int {
        auto bit = [&](int mask, int pos) -> int {
            return (0 <= pos && pos < 2 * n) ? (mask >> pos) & 1 : 0;
        };

        int m = 0;
        for (int j = 0; j <= n - b; j++) {
            int l = bit(mask, 2 * j + b - 1), r = (j < n) ? bit(mask, 2 * j + b) : 0;
            m |= (l ^ r) << j;
        }
        return m;
    };

    for (int b = 0; b <= 1; b++) {
        states[b].resize(1 << (n + 1 - b));
        for (int k = 0; k < all; k++) states[b][masks[b][k] = boundary(k, b)].emplace_back(k);
    }

    vector<vector<long long>> dp(n + 1, vector<long long>(all, 0));
    for (int k = 0; k < all; k++) dp[0][k] = !masks[1][k];
    for (int i = 0; i < n; i++)
        for (int k = 0; k < all; k++)
            if (dp[i][k]) {
                int b = i & 1;
                for (int l : states[b][masks[b][k]]) {
                    for (int j = 0; j < n - b; j++) {
                        if (a[i][j] == -1) continue;
                        if (a[i][j] != popcount((unsigned) (k >> (2 * j + b)) & 3) + popcount((unsigned) (l >> (2 * j + b)) & 3) + popcount((unsigned) (masks[b][k] >> j) & 3)) goto next;
                    }
                    dp[i + 1][l] += dp[i][k];
                    next:;
                }
            }

    auto count = 0LL;
    for (int k = 0; k < all; k++)
        if (!masks[1][k]) count += dp[n][k];
    cout << count;
}
