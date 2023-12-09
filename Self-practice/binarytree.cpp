#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 21092013;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int tc = 0; tc < n; tc++) {
        string s, t;
        cin >> s >> t;

        stack<char> st;
        for (char &c : s) {
            if (c == 'U' && !st.empty()) st.pop();
            else st.emplace(c);
        }

        vector<int> left(t.size()), right(t.size()), dp(t.size() + 2);
        int l = t.size(), r = t.size();
        for (int i = t.size() - 1; i >= 0; i--) {
            left[i] = l;
            right[i] = r;
            if (t[i] == 'L') l = i;
            else if (t[i] == 'R') r = i;
        }

        dp[t.size()] = 1;
        for (int i = t.size() - 1; i >= 0; i--)
            dp[i] = (t[i] == 'U' ? dp[i + 1] : 2 * dp[i + 1] - dp[(t[i] == 'L' ? left[i] : right[i]) + 1]) % MODULO;

        int nodes = dp[0];
        for (int i = 0; i < t.size() && !st.empty(); i++)
            if (t[i] == 'U') {
                nodes = (++nodes + dp[(st.top() == 'L' ? right[i] : left[i]) + 1]) % MODULO;
                st.pop();
            }

        cout << "Case " << tc + 1 << ": " << (nodes + MODULO) % MODULO << "\n";
    }
}
