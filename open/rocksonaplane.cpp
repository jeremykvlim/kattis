#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        int h1 = n / 2, h2 = (n + 1) / 2, total = 0;
        vector<bitset<(int) 24e3 + 1>> dp(h2 + 1, 0);
        dp[0][0] = true;
        while (n--) {
            int w;
            cin >> w;

            total += w;
            for (int h = h2; h; h--) dp[h] |= dp[h - 1] << w;
        }

        auto find = [&](int h) -> int {
            for (int w = total / 2; ~w; w--)
                if (dp[h][w]) return w;
            return 0;
        };
        int l = find(h1), d1 = abs(total - 2 * l), r = find(h2), d2 = abs(total - 2 * r);
        if (d1 > d2) swap(l, r);
        cout << l << " " << total - l << "\n";
    }
}
