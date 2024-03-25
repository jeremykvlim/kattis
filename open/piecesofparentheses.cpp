#include <bits/stdc++.h>
using namespace std;

bool cmp(tuple<int, int, int, int> t1, tuple<int, int, int, int> t2) {
    auto [o1, c1, d1, l1] = t1;
    auto [o2, c2, d2, l2] = t2;
    if ((d1 > 0) ^ (d2 > 0)) return d2 < d1;
    else if (d1 > 0) return c1 == c2 ? d2 < d1 : c1 < c2;
    else return o1 == o2 ? d2 < d1 : o2 < o1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, total = 0;
    cin >> n;

    vector<tuple<int, int, int, int>> pieces(n);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        total += s.length();
        int open = 0, close = 0;
        for (char c : s) {
            if (c == '(') open++;
            else if (open) open--;
            else close++;
        }

        pieces[i] = make_tuple(open, close, open - close, s.length());
    }
    sort(pieces.begin(), pieces.end(), cmp);

    vector<int> dp(total, -1);
    dp[0] = 0;
    for (auto [o, c, d, l] : pieces)
        for (int i = (d <= 0 ? c : total - 1); (d <= 0 ? i < total : i >= c); (d <= 0 ? i++ : i--))
            if (dp[i] >= 0 && i + d >= 0 && i + d < total && dp[i + d] < dp[i] + l) dp[i + d] = dp[i] + l;

    cout << dp[0];
}
