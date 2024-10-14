#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 4>> pieces(n);
    int total = 0;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        total += s.size();
        int open = 0, close = 0;
        for (char c : s) {
            if (c == '(') open++;
            else if (open) open--;
            else close++;
        }

        pieces[i] = {open, close, open - close, (int) s.size()};
    }
    sort(pieces.begin(), pieces.end(), [](auto a1, auto a2) {
        if ((a1[2] > 0) ^ (a2[2] > 0)) return a2[2] < a1[2];
        else if (a1[2] > 0) return a1[1] == a2[1] ? a2[2] < a1[2] : a1[1] < a2[1];
        else return a1[0] == a2[0] ? a2[2] < a1[2] : a2[0] < a1[0];
    });

    vector<int> dp(total, -1);
    dp[0] = 0;
    for (auto [o, c, d, l] : pieces)
        for (int i = (d <= 0 ? c : total - 1); (d <= 0 ? i < total : i >= c); (d <= 0 ? i++ : i--))
            if (dp[i] >= 0 && i + d >= 0 && i + d < total && dp[i + d] < dp[i] + l) dp[i + d] = dp[i] + l;

    cout << dp[0];
}
