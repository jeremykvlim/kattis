#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<long long> a(n), b(m), c(m);
    for (auto &ai : a) cin >> ai;
    for (int i = 0; i < m; i++) cin >> b[i] >> c[i];
    sort(a.begin(), a.end());

    vector<long long> suff(n + 1, 0);
    for (int i = n - 1; ~i; i--) suff[i] = suff[i + 1] + min(a[i], (long long) k);

    auto x = min(b[0] + suff[0], c[0]);
    for (auto i = 0LL, sum = 0LL; i < n; i++) {
        auto d = x - b[0] - sum - suff[i + 1];
        if (d < 0) continue;

        a[i] -= d;
        sum += d;
    }

    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];

    long long l = -1, r = 1e13, mid;
    while (l + 1 < r) {
        mid = (l + r) / 2;

        vector<long long> diff(m - 1);
        for (int i = 1; i < m; i++) diff[i - 1] = min(c[i], mid) - b[i];
        sort(diff.begin(), diff.end());

        auto total = 0LL;
        for (int i = 1; i < m; i++) total += diff[i - 1];

        auto cost = LLONG_MAX;
        for (int i = 0, j = m - 1; i <= n; i++) {
            auto money = (long long) k * (n - i);
            while (j && diff[j - 1] > money) total -= diff[--j];
            cost = min(cost, pref[i] + total + (m - 1 - j) * money);
        }

        if (cost >= pref[n]) r = mid;
        else l = mid;
    }

    cout << x - r;
}
