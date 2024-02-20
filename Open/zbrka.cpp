#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int f(int n, int c, vector<vector<int>> &dp) {
    if (!c) return 1;
    if (c < 0 || !n) return 0;
    if (dp[n][c] != -1) return dp[n][c];

    int r = (f(n, c - 1, dp) - f(n - 1, c - n, dp) + f(n - 1, c, dp)) % MODULO;
    if (r < 0) r += MODULO;

    return dp[n][c] = r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, c;
    cin >> n >> c;

    vector<vector<int>> dp(n + 1, vector<int>(c + 1, -1));

    for (int i = 1; i <= n; i++) f(i, c, dp);
    cout << f(n, c, dp);
}
