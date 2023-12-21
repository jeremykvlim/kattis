#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s;
        cin >> s;

        s += "0";
        int n = ((s.size() ^ (s.size() - 1)) + 1) / 2;
        for (int i = 1; i < s.size() / n; i++) {
            for (int j = 0; j < n - 1; j++)
                if (s[i * n + j] != s[i * n - j - 2]) {
                    cout << "LIVES\n";
                    goto next;
                }

            if (s[i * n - 1] != '0') {
                cout << "LIVES\n";
                goto next;
            }
        }

        cout << "DIES\n";
        next:;
    }
}
