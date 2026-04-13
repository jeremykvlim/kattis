#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, L;
    cin >> n >> L;

    vector<int> a(n), b(n);
    for (auto &ai : a) cin >> ai;
    for (auto &bi : b) cin >> bi;

    vector<long double> pref(n + 1);
    long double l = 0, r = 1e9, m;
    while (l + 1e-9 < r && l + l * 1e-9 < r) {
        m = l + (r - l) / 2;

        auto valid = [&]() {
            pref[0] = 0;
            for (int i = 1; i <= n; i++) pref[i] = pref[i - 1] + a[i - 1] - m * b[i - 1];

            auto s = 0.L;
            for (int i = L; i <= n; i++) {
                s = min(s, pref[i - L]);
                if (pref[i] >= s) return true;
            }
            return false;
        };

        if (valid()) l = m;
        else r = m;
    }
    cout << fixed << setprecision(6) << l;
}