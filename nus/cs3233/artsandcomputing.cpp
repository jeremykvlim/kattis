#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    string t;
    int c = 0, a = 0;
    for (char ch : s)
        if (ch != '.') {
            (ch == 'C' ? c : a)++;
            t += ch;
        }

    if (a < c) {
        cout << "NO";
        exit(0);
    }

    if (s == t) {
        for (int i = 0; i < n; i++) {
            char l = s[2 * i], r = s[2 * i + 1];
            if ((l == 'C' && r != 'A') || (r == 'C' && l != 'A')) {
                cout << "NO";
                exit(0);
            }
        }
        cout << "YES\n" << s;
        exit(0);
    }

    int m = t.size();
    vector<int> dp(m + 1, 1e9), state(m + 1, 0);
    dp[0] = 0;
    for (int i = 1; i <= m; i++) {
        if (t[i - 1] == 'A' && dp[i] > dp[i - 1] + 1) {
            dp[i] = dp[i - 1] + 1;
            state[i] = 1;
        }

        if (i >= 2 && !(t[i - 2] == 'C' && t[i - 1] == 'C'))
            if (dp[i] > dp[i - 2] + 1) {
                dp[i] = dp[i - 2] + 1;
                state[i] = 2;
            }
    }

    if (dp[m] > n) {
        cout << "NO";
        exit(0);
    }

    vector<string> arrangement;
    for (int i = m; i;)
        if (state[i] == 1) {
            arrangement.emplace_back("A.");
            i--;
        } else {
            arrangement.emplace_back(t.substr(i - 2, 2));
            i -= 2;
        }
    reverse(arrangement.begin(), arrangement.end());
    while (arrangement.size() < n) arrangement.emplace_back("..");

    cout << "YES\n";
    for (auto arr : arrangement) cout << arr;
}
