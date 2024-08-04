#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    while (k--) {
        string a, s, t;
        cin >> a >> s >> t;

        if (s == t) {
            cout << "1\n";
            next:;
            continue;
        }

        if (s.size() > t.size()) swap(s, t);
        int n = s.size(), m = t.size();

        for (int i = 0; i <= m - n; i++)
            if (t.substr(i, n) == s) {
                cout << "1\n";
                goto next;
            }

        if (a.size() != 2) {
            cout << "0\n";
            goto next;
        }

        auto check = [&]() -> bool {
            for (int c = 0; c < 2; c++)
                if (s[n - 1] == a[c ^ 1]) {
                    int len_pref = 0, len_suff = 0;
                    for (int i = 0; i < n - 1 && s[i] == a[c]; i++) len_pref++;
                    for (int i = m - 1; ~i && t[i] == a[c]; i--) len_suff++;

                    if (len_pref == n - 1 && len_suff < m && len_pref <= len_suff) return true;
                }

            return false;
        };

        if (check()) {
            cout << "1\n";
            goto next;
        }

        reverse(s.begin(), s.end());
        reverse(t.begin(), t.end());

        if (check()) {
            cout << "1\n";
            goto next;
        }

        cout << "0\n";
    }
}
