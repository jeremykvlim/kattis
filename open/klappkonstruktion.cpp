#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b, c;
    cin >> n >> a >> b >> c;

    vector<array<int, 3>> states;
    vector<vector<vector<int>>> indices(n / 2 + 1, vector<vector<int>>(n / 3 + 1, vector<int>(n / 3 + 1, -1)));
    for (int x = 0; 2 * x <= n; x++)
        for (int y = 0; 3 * y <= n; y++)
            for (int z = 0; 3 * z <= n; z++)
                if (n >= 2 * x + 3 * (y + z)) {
                    states.push_back({x, y, z});
                    indices[x][y][z] = states.size() - 1;
                }

    int s = states.size();
    vector<vector<array<int, 3>>> next(s);
    for (int i = 0; i < s; i++) {
        auto [x, y, z] = states[i];
        int remaining = n - 2 * x - 3 * (y + z);
        for (int u = 0; 2 * u <= remaining; u++)
            for (int v = 0; 2 * u + 3 * v <= remaining; v++) next[i].push_back({u, v, y});
    }

    vector<vector<long long>> dp(s, vector<long long>(b + 1, 0)), temp(s, vector<long long>(b + 1));
    dp[indices[0][0][0]][0] = 1;
    for (int days = 0;; days++) {
        if (n * days >= a + 4 * b + 9 * c && ((dp[indices[0][0][0]][b] >> c) & 1)) {
            cout << days;
            exit(0);
        }

        for (auto &row : temp) fill(row.begin(), row.end(), 0);
        for (int i = 0; i < s; i++)
            for (auto [x, y, z] : next[i])
                for (int j = 0; j + x <= b; j++) temp[indices[x][y][z]][j + x] |= (dp[i][j] << y) & ((1LL << (c + 1)) - 1);
        dp = temp;
    }
}
