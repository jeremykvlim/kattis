#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    k = min(k, 29);

    vector<pair<char, int>> cards(n);
    for (auto &[c, v] : cards) cin >> c >> v;
    cards.emplace_back('m', 1);

    vector<vector<long long>> dp(k + 1, vector<long long>(n + 1, 0));
    vector<long long> add, pref(n, 0), temp(n);
    int mul_count = 0, add_count_curr = 0;
    for (auto [c, v] : cards)
        if (c == 'a') add.emplace_back(v);
        else {
            mul_count = min(mul_count + 1, k);
            sort(add.rbegin(), add.rend());
            for (int i = 0; i < add.size(); i++) pref[i + 1] = pref[i] + add[i];

            int add_count_next = add_count_curr + add.size();
            for (int i = 0; i <= mul_count; i++) {
                auto dnc = [&](auto &&self, int l1, int r1, int l2, int r2) {
                    if (l1 > r1) return;

                    int mid1 = l1 + (r1 - l1) / 2, mid2 = -1;
                    temp[mid1] = -1;
                    for (int j = max(l2, mid1 - (int) add.size()); j <= min(mid1, r2); j++)
                        if (temp[mid1] < dp[i][j] + pref[mid1 - j]) {
                            temp[mid1] = dp[i][j] + pref[mid1 - j];
                            mid2 = j;
                        }
                    self(self, l1, mid1 - 1, l2, mid2);
                    self(self, mid1 + 1, r1, mid2, r2);
                };
                dnc(dnc, 0, add_count_next, 0, min(add_count_curr, n - i));
                for (int j = 1; j <= min(add_count_next, n - i); j++) dp[i][j] = temp[j];
            }

            for (int i = mul_count; i; i--)
                for (int j = 1; j <= min(add_count_next, n - i); j++) {
                    if (!dp[i - 1][j]) break;
                    dp[i][j] = max(dp[i][j], dp[i - 1][j] * v);
                }

            add_count_curr = add_count_next;
            add.clear();
        }

    vector<long long> score(n + 1, 0);
    for (int i = 0; i <= k; i++)
        for (int j = 0; j <= n - i; j++) score[i + j] = max(score[i + j], dp[i][j]);

    for (int i = 1; i <= n; i++) {
        score[i] = max(score[i], score[i - 1]);
        cout << score[i] << "\n";
    }
}
