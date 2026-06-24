#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> m(n), pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        cin >> m[i];

        pref[i + 1] = pref[i] + m[i];
    }

    int l = 0, r = pref[n], mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        cout << "? ";
        for (int i = 0; i < n; i++) cout << min(m[i], max(0, mid - pref[i])) << " ";
        cout << "\n" << flush;

        string s;
        cin >> s;

        if (s == "Villa") r = mid;
        else l = mid;
    }

    int e = upper_bound(pref.begin(), pref.end(), r - 1) - pref.begin();
    cout << "! " << e << " " << r - pref[e - 1];
}
