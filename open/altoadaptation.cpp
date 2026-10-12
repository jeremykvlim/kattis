#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, L, H;
    cin >> n >> L >> H;

    deque<int> dq{0};
    deque<pair<int, int>> mono_dec, mono_inc;
    vector<int> dp(n + 1, 0);
    dp[0] = n;
    for (int i = 1, j = 0, c = 0; i <= n; i++) {
        int a;
        cin >> a;

        int l = (131 + L - a) / 12, r = (120 + H - a) / 12;
        while (!mono_dec.empty() && mono_dec.back().first <= l) mono_dec.pop_back();
        while (!mono_inc.empty() && mono_inc.back().first >= r) mono_inc.pop_back();
        mono_dec.emplace_back(l, i);
        mono_inc.emplace_back(r, i);

        while (mono_dec.front().first > mono_inc.front().first) {
            c++;
            if (mono_dec.front().second == c) mono_dec.pop_front();
            if (mono_inc.front().second == c) mono_inc.pop_front();
        }

        int k = min(i, dp[i - 1] + 1);
        while (!dq.empty() && dq.front() < c) dq.pop_front();
        while (dq.empty() || dp[dq.front()] < k) {
            k--;
            j++;
            if (j < c) continue;

            while (!dq.empty() && dp[dq.back()] <= dp[j]) dq.pop_back();
            dq.emplace_back(j);
        }
        dp[i] = k;
    }
    cout << dp[n];
}
