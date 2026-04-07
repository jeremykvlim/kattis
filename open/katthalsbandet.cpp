#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b;
    string s;
    cin >> n >> a >> b >> s;

    vector<int> pref(n + 1, 0), pref_min(n + 1, 0), suff_min(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + (s[i] == '(' ? 1 : -1);
        pref_min[i + 1] = min(pref_min[i], pref[i + 1]);
    }
    suff_min[n] = pref[n];
    for (int i = n - 1; ~i; i--) suff_min[i] = min(suff_min[i + 1], pref[i]);

    auto save = LLONG_MIN;
    for (int i = 0; i < n; i++) {
        int sl = max(0, pref[i] - pref_min[i]), sr = max(0, pref[i] - suff_min[i + 1]), ml = sl - pref[i], mr = sr + pref[n] - pref[i], r = min((n - i + pref[n] - pref[i]) / 2, (i - pref[i]) / 2);
        auto update = [&](int x) {
            if (0 <= x && x <= r) save = max(save, (long long) (a - b) * (i - sl - abs(x - ml)) + (long long) a * (n - i - sr - abs(x - mr)));
        };
        update(0);
        update(ml);
        update(mr);
        update(r);
    }
    cout << (long long) n * a - save;
}
