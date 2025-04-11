#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, t;
    cin >> n >> l >> t;

    vector<string> words(n);
    for (auto &wi : words) {
        cin >> wi;

        if (wi.size() + 1 > l * t) {
            cout << "/ff";
            exit(0);
        }
    }

    vector<pair<int, vector<string>>> dp(n + 1, {(int) 1e9, {}});
    dp[n].first = 0;
    for (int i = n - 1; ~i; i--) {
        string line;
        for (int j = i; j < n; j++) {
            if (j == i) line = words[i];
            else line += " " + words[j];

            if (line.size() + 1 > l * t) break;

            if (dp[j + 1].first == 1e9 && j < n - 1) continue;

            int c = dp[j + 1].first + 1;
            auto temp = dp[j + 1].second;
            temp.insert(temp.begin(), line);

            if (dp[i].first > c) {
                dp[i].first = c;
                dp[i].second = temp;
            } else if (dp[i].first == c) dp[i].second = min(dp[i].second, temp);
        }
    }

    if (dp[0].first == 1e9) cout << "/ff";
    else
        for (auto line : dp[0].second) cout << line << "\n";
}
