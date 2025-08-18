#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 3>> sections;
    for (int i = 0; i < n; i++) {
        string s;
        int k;
        cin >> s >> k;

        while (k--) {
            string a, b;
            cin >> a >> b;

            auto parse = [&](string t) {
                int colon = t.find(':');
                return stoi(t.substr(0, colon)) * 60 + stoi(t.substr(colon + 1));
            };

            sections.push_back({i, parse(a), parse(b)});
        }
    }

    int size = sections.size();
    vector<vector<int>> adj_list(size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (sections[i][0] != sections[j][0])
                if (sections[i][2] + 15 <= sections[j][1]) adj_list[i].emplace_back(j);

    vector<vector<int>> dp(1 << n, vector<int>(size, -1));
    for (int i = 0; i < size; i++) dp[1 << sections[i][0]][i] = sections[i][1];

    for (int m1 = 0; m1 < 1 << n; m1++)
        for (int i = 0; i < size; i++)
            for (int j : adj_list[i]) {
                if ((m1 >> sections[j][0]) & 1) continue;
                int m2 = m1 | (1 << sections[j][0]);
                dp[m2][j] = max(dp[m2][j], min(dp[m1][i], sections[j][1]));
            }

    int time = INT_MAX;
    for (int i = 0; i < size; i++)
        if (~dp.back()[i]) time = min(time, sections[i][2] - dp.back()[i]);
    cout << time;
}
