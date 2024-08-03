#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> sieve(int n) {
    vector<vector<int>> pfs(n + 1);
    for (int p = 2; p <= n; p == 2 ? p++ : p += 2)
        if (pfs[p].empty())
            for (auto j = (long long) 2 * p; j <= n; j += p) pfs[j].emplace_back(p);

    return pfs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto pfs = sieve(1e4 + 7);
    vector<vector<int>> dp(1e4 + 8, vector<int>(3, 1e9));
    for (int p = 2, prev = 1; p <= 1e4 + 7; p == 2 ? p++ : p += 2) {
        dp[p][0] = 1;

        if (pfs[p].empty()) {
            for (int j = p - 1; j > prev; j--) {
                int k = j + 1;
                dp[j][0] = dp[k][2];
                for (int pf: pfs[j])
                    if (dp[j][0] >= min(j / pf, dp[j / pf][2])) {
                        dp[j][0] = min(j / pf, dp[j / pf][2]);
                        k = j / pf;
                    }
                dp[j][1] = dp[k][0];
                dp[j][2] = dp[k][1];
            }
            prev = p;
        }
    }

    int n;
    cin >> n;

    unordered_map<char, int> indices{{'O', 0}, {'E', 1}, {'I', 2}};
    vector<int> scores(3, 0);
    while (n--) {
        char c;
        int i;
        cin >> c >> i;

        for (int j = 0; j < 3; j++) scores[(indices[c] + j) % 3] += min(i, dp[i][j]);
    }
  
    for (int s : scores) cout << s << " ";
}
