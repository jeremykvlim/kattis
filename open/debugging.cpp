#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, p;
    cin >> n >> r >> p;

    vector<long long> memo(n + 1);
    auto dp = [&](auto &&self, int n) -> long long {
        if (n <= 1) return 0;
        if (memo[n]) return memo[n];

        auto t = (long long) p * (n - 1);
        for (int i = 2; i < n; i = ceil((double) n / (ceil((double) n / i) - 1))) t = min(t, (long long) p * (i - 1) + self(self, ceil((double) n / i)));
        return memo[n] = t + r;
    };
    cout << dp(dp, n);
}
