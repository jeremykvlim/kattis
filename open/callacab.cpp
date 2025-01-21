#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n;
    cin >> t >> n;

    vector<pair<int, int>> modes(t);
    for (auto &[dmin, a] : modes) cin >> dmin >> a;
    sort(modes.begin(), modes.end());

    vector<pair<int, int>> candidates;
    for (auto [dmin, a] : modes) {
        if (!candidates.empty() && candidates.back().second >= a) continue;
        candidates.emplace_back(dmin, a);
    }

    vector<pair<int, int>> relative(n - 1);
    for (auto &[d, h] : relative) cin >> d >> h;
    relative.emplace_back(0, 0);

    int max_degree = 360000;
    auto normalize = [&](int &x) {
        x = (x + max_degree) % max_degree;
    };

    vector<int> dp(n, 1e9);
    dp[n - 1] = 0;
    for (int i = n - 2; ~i; i--) {
        set<int> angles{0};
        multiset<int> diff{max_degree};
        int l = i + 1, r = i + 1, d = relative[i].first, a1 = 0;
        for (auto [dmin, a] : candidates) {
            while (l < n - 1 && d < dmin) d += relative[l++].first;
            if (d < dmin) break;

            while (r < n && a + *diff.rbegin() >= max_degree) {
                a1 += relative[r].second;
                normalize(a1);
                r++;
                if (angles.count(a1)) continue;

                angles.emplace(a1);
                auto it = angles.find(a1);
                int a0 = it == angles.begin() ? *angles.rbegin() : *prev(it),
                    a2 = ++it == angles.end() ? *angles.begin() : *it,
                    diff0 = a2 - a0,
                    diff1 = a1 - a0,
                    diff2 = a2 - a1;

                normalize(diff0);
                normalize(diff1);
                normalize(diff2);

                if (diff.size() == 1) diff.clear();
                else diff.erase(diff.find(diff0));

                diff.emplace(diff1);
                diff.emplace(diff2);
            }
            for (int j = l; j < r; j++) dp[i] = min(dp[i], dp[j] + 1);
        }
    }

    if (dp[0] != 1e9) cout << dp[0];
    else cout << "IMPOSSIBLE";
}
