#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> colors(n);
    for (int &c : colors) cin >> c;

    vector<vector<vector<int>>> memo(n, vector<vector<int>>(n, vector<int>(k, -1)));
    auto dp = [&](auto &&self, int l, int r, int count = 0) {
        if (l > r) return 0;
        if (l == r) return k - count - 1;
        if (memo[l][r][count] != -1) return memo[l][r][count];

        memo[l][r][count] = INT_MAX;
        if (count < k - 1) memo[l][r][count] = self(self, l, r, count + 1) + 1;
        if (count == k - 1) memo[l][r][count] = self(self, l + 1, r);

        for (int m = l + 1; m <= r; m++)
            if (colors[l] == colors[m]) memo[l][r][count] = min(memo[l][r][count], self(self, l + 1, m - 1) + self(self, m, r, min(k - 1, count + 1)));

        return memo[l][r][count];
    };
    cout << dp(dp, 0, n - 1);
}
