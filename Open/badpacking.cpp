#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> weights(n);
    for (auto &w : weights) cin >> w;

    sort(weights.begin(), weights.end());
    vector<bool> dp(c + 1, false);
    dp[0] = true;
    int total = accumulate(weights.begin(), weights.end(), 0);
    for (int i = c, j = n - 1; i; i--) {
        for (; ~j && weights[j] == i + 1; j--) {
            total -= i + 1;
            for (int k = c; k >= i + 1; k--)
                if (dp[k - i - 1]) dp[k] = true;
        }

        if (c - i - total >= 0 && dp[c - i - total]) {
            cout << c - i;
            exit(0);
        }
    }
}
