#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int &ai : a) cin >> ai;
    for (int &bi : b) cin >> bi;

    vector<int> offsets(n);
    for (int i = 0; i < n; i++) offsets[i] = lower_bound(b.begin(), b.end(), a[i] + 1) - b.begin() - 1 - i;

    vector<int> pref{INT_MAX}, suff{INT_MIN};
    for (int i = 0; i < n - 1; i++) {
        pref.emplace_back(min(pref.back(), offsets[i]));
        suff.emplace_back(max(suff.back(), -offsets[n - 1 - i]));
    }

    int shifts = 0;
    for (int i = 0; i < n; i++) shifts += pref[i] >= n - i && suff[n - 1 - i] <= i;
    cout << shifts;
}
