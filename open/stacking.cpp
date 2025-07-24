#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int n, x = 1; cin >> n; x++) {
        vector<vector<int>> indices(1e4 + 1);
        for (int i = 0; i < n; i++) {
            int h;
            cin >> h;

            int prev = -1;
            while (h--) {
                int d;
                cin >> d;

                if (prev != d) {
                    prev = d;
                    indices[d].emplace_back(i);
                }
            }
        }

        vector<vector<int>> memo(n, vector<int>(1e4 + 1, -1));
        auto dp = [&](auto &&self, int i, int d = 0) {
            if (d == 10001) return 0;
            if (~memo[i][d]) return memo[i][d];
            if (indices[d].empty() || (indices[d].size() == 1 && indices[d][0] == i)) return memo[i][d] = self(self, i, d + 1);

            memo[i][d] = 1e9;
            bool extra = find(indices[d].begin(), indices[d].end(), i) != indices[d].end();
            for (int j : indices[d]) memo[i][d] = min(memo[i][d], self(self, j, d + 1) + (int) indices[d].size() - (i != j && extra));
            return memo[i][d];
        };

        int ops = 1e9;
        for (int i = 0; i < n; i++) ops = min(ops, dp(dp, i));
        cout << "Case " << x << ": " << 2 * ops - n + 1 << "\n";
    }
}
