#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int c, n1, n5, n10;
        cin >> c >> n1 >> n5 >> n10;

        int total = n1 + 5 * n5 + 10 * n10;
        n5 += n10;

        vector<int> dp((n5 + 1) * (n10 + 1), 1e9), temp((n5 + 1) * (n10 + 1), 1e9);
        auto index = [&](int f, int t) {
            return f * (n10 + 1) + t;
        };
        dp[index(n5 - n10, n10)] = 0;
        for (int cokes = 0; cokes < c; cokes++) {
            fill(temp.begin(), temp.end(), 1e9);
            int remaining = total - 8 * cokes;

            for (int fives = 0; fives <= n5; fives++) {
                for (int tens = 0; tens <= n10; tens++) {
                    int coins = dp[index(fives, tens)];
                    if (coins == 1e9) continue;

                    int ones = remaining - 5 * fives - 10 * tens;
                    if (ones < 0) continue;

                    auto update = [&](int fives, int tens, int coins) {
                        int i = index(fives, tens);
                        temp[i] = min(temp[i], coins);
                    };

                    if (tens) update(fives, tens - 1, coins + 1);
                    if (fives > 1) update(fives - 2, tens, coins + 2);
                    if (fives && ones > 2) update(fives - 1, tens, coins + 4);
                    if (ones >= 8) update(fives, tens, coins + 8);
                    if (tens && ones > 2 && fives + 1 <= n5) update(fives + 1, tens - 1, coins + 4);
                }
            }
            dp = temp;
        }
        cout << *min_element(dp.begin(), dp.end()) << "\n";
    }
}
