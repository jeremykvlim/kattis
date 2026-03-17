#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    int sum = 0;
    vector<pair<int, int>> l, r;
    while (n--) {
        int ti, fi;
        cin >> ti >> fi;

        sum += fi;
        if (ti < fi) l.emplace_back(ti, fi);
        else r.emplace_back(-fi, ti);
    }
    sort(l.begin(), l.end());
    sort(r.begin(), r.end());

    int extension = 0;
    vector<int> dp(sum + 1, 1e9);
    dp[0] = 0;
    for (auto [ti, fi] : l)
        for (int f = sum - fi; ~f; f--)
            if (dp[f] != 1e9 && dp[f] + ti <= t + f) {
                dp[f + fi] = min(dp[f + fi], dp[f] + ti);
                extension = max(extension, f + fi);
            }
    for (auto [fi, ti] : r)
        for (int f = sum + fi; ~f; f--)
            if (dp[f] != 1e9 && dp[f] + ti <= t + f) {
                dp[f - fi] = min(dp[f - fi], dp[f] + ti);
                extension = max(extension, f - fi);
            }
    cout << t + extension;
}
