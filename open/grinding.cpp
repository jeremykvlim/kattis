#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long b;
    cin >> n >> b;

    vector<int> indices(n + 2);
    vector<long long> strengths, sums;
    for (int i = 1; i <= n; i++) {
        int m;
        cin >> m;

        indices[i] = strengths.size();
        auto pref = 0LL, s_max = LLONG_MIN;
        while (m--) {
            long long s, e;
            cin >> s >> e;

            s_max = max(s_max, s - pref);
            pref += e;
            if (indices[i] == strengths.size() || strengths.back() != s_max) {
                strengths.emplace_back(s_max);
                sums.emplace_back(pref);
            } else sums.back() = pref;
        }
    }
    indices[n + 1] = strengths.size();

    auto attacks = LLONG_MAX;
    for (long long a = 0, s = 1, d = 0, i = 1;;) {
        if (s >= b) {
            attacks = min(attacks, a);
            break;
        }

        if (d) attacks = min(attacks, a + (b - s + d - 1) / d);
        if (i > n) break;

        int l = indices[i], r = indices[i + 1];
        if (s >= strengths[r - 1]) {
            a++;
            s += sums[r - 1];
            d = max(d, sums[r - 1]);
            i++;
            continue;
        }

        int p = upper_bound(strengths.begin() + l, strengths.begin() + r, s) - strengths.begin() - l;
        auto g = max(d, p ? sums[l + p - 1] : 0LL), k = (min(b, strengths[l + p]) - s + g - 1) / g;
        a += k;
        s += k * g;
    }
    cout << attacks;
}
