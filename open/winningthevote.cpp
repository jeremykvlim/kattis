#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string S;
    cin >> n >> S;

    string voters;
    vector<int> tellers;
    for (int count = 0; char &c : S) {
        if (c == '0') tellers.emplace_back(count);
        else {
            voters += c;
            count++;
        }
    }
    if (tellers.empty()) {
        cout << "impossible";
        exit(0);
    }

    int m = voters.size();
    vector<int> delta(m + 1, 0);
    for (int i = 0; i < m; i++) delta[i + 1] = delta[i] + (voters[i] == '1' ? 1 : -1);

    vector<vector<int>> dist(3, vector<int>(m + 1, 1e9));
    for (int i = 0; i <= m; i++) dist[(delta[i] > 0) - (delta[i] < 0) + 1][i] = 0;
    for (int s = 0; s < 3; s++) {
        for (int i = 1; i <= m; i++) dist[s][i] = min(dist[s][i], dist[s][i - 1] + 1);
        for (int i = m - 1; ~i; i--) dist[s][i] = min(dist[s][i], dist[s][i + 1] + 1);
    }

    int k = tellers.size();
    vector<int> dp(2 * k + 1, 1e9), temp(2 * k + 1, 1e9);
    dp[k] = 0;
    for (int i = 0; i < k; i++) {
        fill(temp.begin(), temp.end(), 1e9);
        for (int j = -i; j <= i; j++)
            for (int s = -1; s <= 1; s++) temp[j + k + s] = min(temp[j + k + s], dp[j + k] + dist[s + 1][tellers[i]]);
        dp = temp;
    }

    int swaps = *min_element(dp.begin() + k + 1, dp.end());
    if (swaps == 1e9) cout << "impossible";
    else cout << swaps;
}