#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> valid;
    auto dfs = [&](auto &&self, long long x = 0, int digit = 1, int len = 0) -> void {
        auto sq = (__int128) x * x;
        for (int prev = 10; sq; sq /= 10) {
            int d = sq % 10;
            if (prev < d) goto done;
            prev = d;
        }
        valid.emplace_back(x);
        done:;
        if (len == 18) return;
        for (int d = digit; d <= 9; d++) self(self, x * 10 + d, d, len + 1);
    };
    dfs(dfs);
    sort(valid.begin(), valid.end());

    int t;
    cin >> t;

    while (t--) {
        long long x;
        cin >> x;
        cout << upper_bound(valid.begin(), valid.end(), x) - valid.begin() - 1 << "\n";
    }
}
