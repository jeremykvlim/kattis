#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long count(int i, int j, int n, string &anti, vector<vector<int>> &indices, vector<vector<long long>> &dp) {
    if (i == n) return 1;
    if (dp[i][j]) return dp[i][j];

    auto c = 0LL;
    for (auto k : indices[j]) 
        c += (k == anti.size()) ? 0 : count(i + 1, k, n, anti, indices, dp);

    return dp[i][j] = c % MODULO;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        string anti;
        cin >> n >> anti;

        vector<vector<int>> len(anti.size(), vector<int>(2, 0));
        auto prepare = [&](int i, char c) {
            auto s = anti.substr(0, i) + c;

            for (int k = 0; k < s.size(); k++) {
                auto suff = s.substr(k);
                if (suff == anti.substr(0, suff.size())) {
                    len[i][c - '0'] = s.size() - k;
                    break;
                }
            }
        };

        for (int i = 0; i < anti.size(); i++) {
            prepare(i, '0');
            prepare(i, '1');
        }

        vector<vector<long long>> dp(n, vector<long long>(anti.size(), 0));
        cout << count(0, 0, n, anti, len, dp) << "\n";
    }
}
