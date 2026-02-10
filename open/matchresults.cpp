#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        string s;
        cin >> n >> s;

        vector<int> pref(n + 1, 0), alice{-1}, bob{-1};
        for (int i = 0; i < n; i++) {
            pref[i + 1] = pref[i] + (s[i] == 'A');
            if (s[i] == 'A') alice.emplace_back(i);
            else bob.emplace_back(i);
        }

        vector<int> values;
        for (int k = 1; k <= n; k++) {
            for (int l = 0; l < n;) {
                int a = pref[l] + k, b = l - pref[l] + k, r = min(a < alice.size() ? alice[a] : n, b < bob.size() ? bob[b] : n);
                if (r == n) goto next;
                l = r + 1;
            }
            values.emplace_back(k);
            next:;
        }
        cout << values.size() << "\n";
        for (int k : values) cout << k << " ";
        cout << "\n";
    }
}
