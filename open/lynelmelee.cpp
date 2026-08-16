#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, b;
    cin >> n >> b;

    vector<int> a(n + 1);
    long long total = 0, heavy = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        total += (long long) a[i] * max(5, i);
        if (i >= 5) heavy += a[i] * i;
    }
    total -= b;

    vector<pair<int, int>> candidates;
    for (int i = 1; i < 5; i++)
        for (int two = 0; two <= (n >= 2 && a[2]); two++)
            for (int three = 0; three <= (n >= 3 && a[3]); three++)
                for (int four = 0; four <= (n >= 4 ? min(a[4], 3) : 0); four++) {
                    int gain = 5 - i - 3 * two - 2 * three - four;
                    if (gain <= 0) continue;

                    int s = b + i - 2 * two - 3 * three - 4 * four;
                    if (0 <= s && s <= heavy) candidates.emplace_back(min((long long) s, heavy - s), gain);
                }

    int limit = 0;
    for (auto [s, gain] : candidates) limit = max(limit, s);

    dynamic_bitset<> dp(limit + 1);
    dp[0] = true;
    for (int i = 5; i <= min(n, limit); i++)
        for (int remaining = min(a[i], limit / i), p2 = 1; remaining; p2 *= 2) {
            int use = min(remaining, p2);
            dp |= dp << (use * i);
            remaining -= use;
        }

    int g = 0;
    for (auto [s, gain] : candidates)
        if (dp[s]) g = max(g, gain);
    cout << total + g;
}
