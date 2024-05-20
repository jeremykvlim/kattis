#include <bits/stdc++.h>
using namespace std;

long long count(vector<vector<vector<vector<long long>>>> &dp, string s, int pos = 0, int x = 10, int y = 10, bool less = true) {
    if (pos == s.size()) return 1;
    if (dp[pos][x][y][less] != -1) return dp[pos][x][y][less];

    dp[pos][x][y][less] = 0;
    for (int i = 0; i <= (less ? s[pos] - '0' : 9); i++)
        if (i != x && i != y)
            dp[pos][x][y][less] += (!i && x == 10 && y == 10) ? count(dp, s, pos + 1, x, y, false) : count(dp, s, pos + 1, i, x, less && i == s[pos] - '0');

    return dp[pos][x][y][less];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    cin >> a >> b;

    vector<vector<vector<vector<long long>>>> dp1(18, vector<vector<vector<long long>>>(11, vector<vector<long long>>(11, vector<long long>(2, -1)))),
                                              dp2(18, vector<vector<vector<long long>>>(11, vector<vector<long long>>(11, vector<long long>(2, -1))));
    if (a) a = count(dp1, to_string(a - 1));
    b = count(dp2, to_string(b));
    cout << b - a;
}
