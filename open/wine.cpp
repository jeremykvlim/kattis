#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int wine, sizes;
        cin >> wine >> sizes;
        wine *= 1000;

        vector<pair<int, int>> bottles(sizes);
        int least = INT_MAX;
        for (auto &[l, r] : bottles) {
            cin >> l >> r;

            least = min(least, (r * l) / (r - l));
        }

        if (wine >= least) {
            next:;
            cout << "0\n";
            continue;
        }

        vector<int> dp(least + 1, -1);
        dp[0] = 0;
        for (int i = 0; i <= least; i++)
            if (~dp[i])
                for (auto [l, r] : bottles)
                    if (i + r <= least) dp[i + r] = max(dp[i + r], dp[i] + r - l);

        int bottled = 0;
        for (int i = 0; i <= least; i++)
            if (~dp[i]) {
                if (wine <= i && i <= wine + dp[i]) goto next;
                else if (i <= wine) bottled = max(bottled, i);
            }
        cout << wine - bottled << "\n";
    }
}
