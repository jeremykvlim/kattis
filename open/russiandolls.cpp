#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<array<int, 3>> d(2 * n + 1, {0, 0, 0});
        for (int i = 1; i <= 2 * n; i++) cin >> d[i][0] >> d[i][1] >> d[i][2];
        sort(d.begin(), d.end(), [](auto a1, auto a2) {return a1[0] != a2[0] ? a1[0] < a2[0] : a1[1] != a2[1] ? a1[1] < a2[1] : a1[2] > a2[2];});

        vector<vector<vector<bool>>> dp(2 * n + 1, vector<vector<bool>>(2 * n + 1, vector<bool>(2 * n + 1, false)));
        auto arrange = [&](auto &&self, bool first, int count = 0, int i = 0, int j = 0) -> bool {
            int k = max(i, j);
            if (2 * count >= d.size() || 2 * (k - count) >= d.size() || dp[i][j][count]) return false;
            if (k++ == d.size() - 1) return true;

            auto valid = [&](int l) {
                return d[k][0] - 2 * d[k][2] >= d[l][0] && d[k][1] - 2 * d[k][2] >= d[l][1];
            };

            if (valid(j) && self(self, first, count, i, k)) {
                if (first) cout << d[k][0] << " " << d[k][1] << " " << d[k][2] << "\n";
                return true;
            }

            if (valid(i) && self(self, first, count + 1, k, j)) {
                if (!first) cout << d[k][0] << " " << d[k][1] << " " << d[k][2] << "\n";
                return true;
            }

            dp[i][j][count] = true;
            return false;
        };

        arrange(arrange, true);
        cout << "-\n";
        arrange(arrange, false);
        cout << "\n";
    }
}
