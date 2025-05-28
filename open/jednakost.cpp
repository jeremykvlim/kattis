#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string t;
    cin >> t;

    int eq = t.find('='), s = stoi(t.substr(eq + 1));
    auto a = t.substr(0, eq);

    int n = a.size();
    vector<vector<int>> dp1(n + 1, vector<int>(s + 1, 0)), dp2(n + 1, vector<int>(s + 1, 0));
    dp1[0][0] = 1;
    for (int i = 0; i < n; i++)
        for (int S = 0; S <= s; S++)
            if (dp1[i][S]) {
                int ss = 0;
                for (int j = 0; i + j < n; j++) {
                    ss = ss * 10 + a[i + j] - '0';
                    if (S + ss > s) break;
                    if (!ss && i + j + 1 < n && a[i + j + 1] == '0') continue;

                    if (!dp1[i + j + 1][S + ss] || dp2[i][S] < dp2[i + j + 1][S + ss]) {
                        dp1[i + j + 1][S + ss] = j + 1;
                        dp2[i + j + 1][S + ss] = dp2[i][S] + 1;
                    }
                }
            }

    vector<bool> visited(n + 1, false);
    for (int i = n, S = s; i;) {
        visited[i] = true;
        int ss = stoi(a.substr(i - dp1[i][S], dp1[i][S]));
        i -= dp1[i][S];
        S -= ss;
    }

    for (int i = 0; i < n; i++) {
        if (visited[i]) cout << "+";
        cout << a[i];
    }
    cout << "=" << s;
}
