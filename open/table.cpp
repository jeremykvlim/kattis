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
        for (int w = X - 2; ~w; w--)
            if (table[w][l]) table[w][l] = table[w + 1][l] + 1;

    for (auto &r : table) {
        stack<pair<int, int>> s;
        s.emplace(0, 0);
        
        r.emplace_back(0);
        for (int i = 0; i < r.size(); i++) {
            auto p = make_pair(i, r[i]);
            while (r[i] < s.top().second) {
                p = s.top();
                s.pop();
                
                dp[p.second][i - p.first]++;
                dp[max(r[i], s.top().second)][i - p.first]--;
            }
            s.emplace(p.first, r[i]);
        }
    }

    for (int w = 0; w <= X; w++) {
        for (int l = Y; l; l--) dp[w][l - 1] += dp[w][l];
        for (int l = Y; l; l--) dp[w][l - 1] += dp[w][l];
    }

    for (int l = 0; l <= Y; l++)
        for (int w = X; w; w--) dp[w - 1][l] += dp[w][l];

    while (d--) {
        int x, y;
        cin >> x >> y;
        cout << dp[x][y] << "\n";
    }
}
