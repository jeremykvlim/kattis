#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    double l = 0, r = INT_MAX, m;
    while (l + l * 1e-11 < r) {
        m = l + (r - l) / 2;

        auto valid = [&]() -> bool {
            vector<double> pref(n);
            pref[0] = a[0] - m;
            for (int i = 1; i < n; i++) pref[i] = pref[i - 1] + a[i] - m;

            auto least = pref[0];
            for (int i = k; i < n; i++) {
                least = min(least, pref[i - k]);
                if (pref[i] - least >= 0) return true;
            }

            return false;
        };

        if (valid()) l = m;
        else r = m;
    }

    cout << fixed << setprecision(6) << l;
}
