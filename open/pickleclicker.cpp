#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    vector<pair<int, int>> pickles(n);
    for (auto &[p, c] : pickles) cin >> p >> c;

    unordered_map<int, int> dp;
    dp[pickles[0].first] = 0;
    for (int time = 1;; time++) {
        unordered_map<int, int> temp;
        for (auto [prod1, total1] : dp) {
            int total2 = prod1 + total1;
            if (!temp.count(prod1) || temp[prod1] < total2) temp[prod1] = total2;

            for (auto [p, c] : pickles)
                if (total2 >= c) {
                    int prod2 = prod1 + p, total3 = total2 - c;
                    if (!temp.count(prod2) || temp[prod2] < total3) temp[prod2] = total3;
                }
        }

        for (auto [prod, total] : temp)
            if (total >= t) {
                cout << time;
                exit(0);
            }

        dp = temp;
    }
}
