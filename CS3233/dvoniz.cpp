#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    vector<long long> a(n + 1, 0), pref(n + 3, LLONG_MAX);
    pref[0] = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pref[i] = pref[i - 1] + a[i];
    }

    for (int i = 0, j = 0; i < n; j = max(j - 1, i + 1), i++) {
        while (pref[j] - pref[i] < 2 * s) j += 2;
        while (pref[(i + j) / 2] - pref[i] > s || pref[j] - pref[(i + j) / 2] > s) j -= 2;
        cout << j - i << "\n";
    }
}
