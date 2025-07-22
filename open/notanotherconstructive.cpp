#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    string s;
    cin >> n >> k >> s;

    vector<int> NA_max(n + 1, 0);
    for (int i = 0; i <= n; i++) NA_max[i] = i * i / 4;

    vector<vector<vector<bitset<2501>>>> dp(n + 1);
    for (int i = 0; i <= n; i++) {
        dp[i].resize(i + 1);
        for (int N = 0; N <= i; N++) {
            dp[i][N].resize(NA_max[i] + 1);
            if (i == n)
                for (auto &bs : dp[n][N]) bs[0] = true;
        }
    }

    for (int i = n - 1; ~i; i--) {
        char c = s[i];
        for (int N1 = 0; N1 <= i; N1++)
            for (int NA1 = 0; NA1 <= NA_max[i]; NA1++)
                for (char ch : (c == '?' ? vector<char>{'N', 'A', 'C', 'Z'} : vector<char>{c})) {
                    int N2 = N1, NA2 = NA1, NAC = 0;
                    if (ch == 'N') N2 = N1 + 1;
                    else if (ch == 'A') {
                        NA2 = NA1 + N1;
                        if (NA2 > NA_max[i + 1]) continue;
                    } else if (ch == 'C') NAC = NA1;

                    if (N2 <= i + 1) dp[i][N1][NA1] |= dp[i + 1][N2][NA2] << NAC;
                }

    }

    if (!dp[0][0][0][k]) {
        cout << -1;
        exit(0);
    }

    auto t = s;
    for (int i = 0, N1 = 0, NA1 = 0; i < n; i++) {
        if (s[i] != '?') {
            char c = s[i];
            if (c == 'N') N1++;
            else if (c == 'A') NA1 += N1;
            else if (c == 'C') k -= NA1;
            t[i] = c;
            continue;
        }

        bool reached = false;
        for (char ch : {'N', 'A', 'C', 'Z'}) {
            int N2 = N1, NA2 = NA1, NAC = 0;
            if (ch == 'N') N2 = N1 + 1;
            else if (ch == 'A') {
                NA2 = NA1 + N1;
                if (NA2 > NA_max[i + 1]) continue;
            } else if (ch == 'C') NAC = NA1;

            if (N2 > i + 1 || k < NAC) continue;

            if (dp[i + 1][N2][NA2][k - NAC]) {
                t[i] = ch;
                N1 = N2;
                NA1 = NA2;
                k -= NAC;
                reached = true;
                break;
            }
        }

        if (!reached) {
            cout << -1;
            exit(0);
        }
    }
    cout << t;
}
