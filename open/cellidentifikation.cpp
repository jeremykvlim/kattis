#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int size1 = 1 << 10, blocks = (n + size1 - 1) / size1;
    vector<vector<dynamic_bitset<>>> comp(blocks, vector<dynamic_bitset<>>(k, dynamic_bitset<>(size1)));
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        for (int j = 0; j < k; j++)
            if (s[j] == '1') comp[i / size1][j][i % size1] = true;
    }

    int q;
    cin >> q;

    int size2 = 10, groups = (k + size2 - 1) / size2;
    vector<vector<int>> queries(q, vector<int>(groups));
    for (int i = 0; i < q; i++) {
        string s;
        cin >> s;

        for (int j = 0; j < k; j++)
            if (s[j] == '1') queries[i][j / size2] |= 1 << (j % size2);
    }

    vector<int> cells(q, -1), active(q);
    iota(active.begin(), active.end(), 0);
    vector<vector<dynamic_bitset<>>> dp(groups, vector<dynamic_bitset<>>(1 << size2, dynamic_bitset<>(size1)));
    for (int b = 0; b < blocks && !active.empty(); b++) {
        for (int g = 0; g < groups; g++) {
            dp[g][0].set();
            dp[g][0] >>= size1 - min(size1, n - b * size1);
            for (int mask = 1; mask < 1 << min(size2, k - g * size2); mask++) dp[g][mask] = dp[g][mask & (mask - 1)] & comp[b][g * size2 + countr_zero((unsigned)mask)];
        }

        vector<int> temp;
        for (int qi : active) {
            auto matches = dp[0][queries[qi][0]];
            for (int g = 1; g < groups && matches.any(); g++) matches &= dp[g][queries[qi][g]];

            if (matches.none()) temp.emplace_back(qi);
            else if (~cells[qi] || matches.count() > 1) cells[qi] = -2;
            else {
                cells[qi] = b * size1 + matches.find_first();
                temp.emplace_back(qi);
            }
        }
        active = temp;
    }

    for (int i : cells)
        if (i == -1) cout << "finns ej\n";
        else if (i == -2) cout << "vet ej\n";
        else cout << i + 1 << "\n";
}