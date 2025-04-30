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

    vector<long long> dp1(n + 1, 0), dp2(n + 1, 0);
    dp2[0] = n;
    deque<int> dq;
    auto count = 0LL;
    for (int j = 1, k = 0; j <= n; j++) {
        int i = n - j;

        int start = lower_bound(times.begin() + i + 1, times.end(), times[i] + t) - times.begin(), end = min(start + c, n);

        for (; k <= n - start; k++) {
            while (!dq.empty() && dp2[dq.back()] <= dp2[k]) dq.pop_back();
            dq.emplace_back(k);
        }

        while (!dq.empty() && dq.front() <= n - end) dq.pop_front();

        if (start + 1 <= end && !dq.empty()) dp1[j] = dp2[dq.front()] + 1 - start;
        dp2[j] = dp1[j] + n - j;
        count = max(count, dp1[j]);
    }
    cout << count;
}
