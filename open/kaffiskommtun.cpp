#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c, t;
    cin >> n >> c >> t;

    vector<int> times(n);
    for (int &ti : times) cin >> ti;
    sort(times.begin(), times.end());

    vector<long long> dp(n + 1, 0);
    dp[0] = n;
    deque<int> dq;
    auto count = 0LL;
    for (int i = 1, j = 0, l = n; i <= n; i++) {
        for (; l - 1 > n - i && times[l - 1] - times[n - i] >= t; l--);
        for (; j <= n - l; j++) {
            while (!dq.empty() && dp[dq.back()] <= dp[j]) dq.pop_back();
            dq.emplace_back(j);
        }

        while (!dq.empty() && dq.front() <= n - min(l + c, n)) dq.pop_front();

        auto curr = 0LL;
        if (!dq.empty()) curr = dp[dq.front()] + 1 - l;
        dp[i] = curr + n - i;
        count = max(count, curr);
    }
    cout << count;
}
