#include <bits/stdc++.h>
using namespace std;

ostream & operator<<(ostream &stream, const __int128 &v) {
    if (!v) return stream << 0;
    if (v == numeric_limits<__int128>::min()) return stream << "-170141183460469231731687303715884105728";

    auto temp = v;
    if (v < 0) {
        stream << '-';
        temp = -temp;
    }

    string s;
    while (temp) {
        s += '0' + (temp % 10);
        temp /= 10;
    }
    reverse(s.begin(), s.end());
    return stream << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, b, s;
    cin >> n >> b >> s;

    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        int l;
        cin >> l;

        pref[i] = pref[i - 1] + l + s;
    }

    vector<__int128> dp(n + 1, 0);
    auto penalty = [&](int i, int j) {
        auto x = pref[j] - pref[i] - s;
        return dp[i] + (__int128) (b - x) * (b - x);
    };

    deque<array<int, 3>> dq;
    for (int i = 0, right = 0; i <= n; i++) {
        while (!dq.empty() && dq.front()[2] < i) dq.pop_front();
        if (!dq.empty() && dq.front()[1] <= i) dp[i] = penalty(dq.front()[0], i);
        for (; right < n && pref[right + 1] <= pref[i] + b + s; right++);

        int left = i + 1;
        while (!dq.empty()) {
            auto [j, l, r] = dq.back();

            if (penalty(i, l) <= penalty(j, l)) dq.pop_back();
            else if (penalty(i, r) > penalty(j, r)) {
                left = r + 1;
                break;
            } else {
                while (l + 1 < r) {
                    int m = l + (r - l) / 2;

                    if (penalty(i, m) <= penalty(j, m)) r = m;
                    else l = m;
                }

                dq.back()[2] = r - 1;
                left = r;
                break;
            }
        }

        if (left <= right) dq.push_back({i, left, right});
    }
    cout << dp[n];
}
