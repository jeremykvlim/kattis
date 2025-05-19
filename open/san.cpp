#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> p10(11, 1);
    for (int i = 1; i <= 10; i++) p10[i] = p10[i - 1] * 10;

    vector<long long> sums, count;
    auto dfs = [&](auto &&self, int len, int pos = 0, long long s = 0, long long c = 1) {
        if (s > 1e10) return;
        if (pos == (len + 1) / 2) {
            sums.emplace_back(s);
            count.emplace_back(c);
            return;
        }
        if (len == 2 * pos + 1)
            for (int i = !pos ? 2 : 0; i <= 18; i += 2) self(self, len, pos + 1, s + p10[pos] * i, c);
        else
            for (int i = !pos; i <= 18; i++) self(self, len, pos + 1, s + (p10[pos] + p10[len - pos - 1]) * i, c * (i > 9 ? 19 - i : i + !!pos));
    };
    for (int i = 1; i <= 10; i++) dfs(dfs, i);

    auto temp = sums;
    sort(sums.begin(), sums.end());
    sums.erase(unique(sums.begin(), sums.end()), sums.end());

    vector<long long> pref(sums.size() + 1, 0);
    for (int i = 0; i < temp.size(); i++) pref[upper_bound(sums.begin(), sums.end(), temp[i]) - sums.begin()] += count[i];

    for (int i = 1; i < sums.size(); i++) {
        auto rev = to_string(sums[i - 1]);
        reverse(rev.begin(), rev.end());
        auto a = sums[i - 1] + stoll(rev);
        if (a <= 1e10) pref[upper_bound(sums.begin(), sums.end(), a) - sums.begin()] += pref[i];
        pref[i] += pref[i - 1];
    }

    int q;
    cin >> q;

    while (q--) {
        long long a, b;
        cin >> a >> b;
        a--;

        cout << pref[upper_bound(sums.begin(), sums.end(), b) - sums.begin()] + b - pref[upper_bound(sums.begin(), sums.end(), a) - sums.begin()] - a << "\n";
    }
}
