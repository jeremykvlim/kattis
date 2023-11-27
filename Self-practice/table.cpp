#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int X, Y, n, d;
    cin >> X >> Y >> n >> d;
    vector<vector<int>> table(X, vector<int>(Y, 1)), dp(X + 1, vector<int>(Y + 1, 0));

    while (n--) {
        int x, xp, y, yp;
        cin >> x >> xp >> y >> yp;
        for (int i = x; i < xp; i++)
            for (int j = y; j < yp; j++) table[i][j] = 0;
    }

    for (int l = 0; l < Y; l++)
        for (int w = X - 2; w >= 0; w--)
            if (table[w][l]) table[w][l] = table[w + 1][l] + 1;

    for (auto &c : table) {
        vector<pair<int, int>> v{{0, 0}};
        c.push_back(0);
        for (int i = 0; i < c.size(); i++) {
            auto sb = make_pair(i, c[i]);
            while (c[i] < v.back().second) {
                sb = v.back();
                v.pop_back();
                dp[sb.second][i - sb.first]++;
                dp[max(c[i], v.back().second)][i - sb.first]--;
            }
            v.emplace_back(sb.first, c[i]);
        }
    }

    for (int w = 0; w <= X; w++) {
        for (int l = Y; l > 0; l--) dp[w][l - 1] += dp[w][l];
        for (int l = Y; l > 0; l--) dp[w][l - 1] += dp[w][l];
    }

    for (int l = 0; l <= Y; l++)
        for (int w = X; w > 0; w--) dp[w - 1][l] += dp[w][l];

    while (d--) {
        int x, y;
        cin >> x >> y;
        cout << dp[x][y] << "\n";
    }
}
