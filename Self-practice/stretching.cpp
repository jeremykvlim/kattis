#include<bits/stdc++.h>
using namespace std;

bool valid(string &s, string &sub, int i, int l, int r, int count, vector<vector<vector<bool>>> &dp, vector<vector<vector<int>>> &visited) {
    if (l > r) return !i;
    if (r - l + 1 - sub.size() + i < 0 || (r - l + 1 - sub.size() + i) % sub.size() || s[l] != sub[i]) return false;
    if (visited[l][r][i] == count) return dp[l][r][i];

    visited[l][r][i] = count;
    if (i == sub.size() - 1) return dp[l][r][i] = valid(s, sub, 0, l + 1, r, count, dp, visited);
    for (int j = l + 1; j <= r; j++)
        if (s[j] == sub[i + 1])
            if (valid(s, sub, 0, l + 1, j - 1, count, dp, visited) && valid(s, sub, i + 1, j, r, count, dp, visited)) return dp[l][r][i] = true;

    return dp[l][r][i] = false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int len = s.size(), count = 0;
    vector<vector<vector<bool>>> dp(len + 1, vector<vector<bool>>(len + 1, vector<bool>(len + 1, false)));
    vector<vector<vector<int>>> visited(len + 1, vector<vector<int>>(len + 1, vector<int>(len + 1, 0)));

    for (int i = 1; i <= len; i++)
        if (!(len % i)) {
            string p, sub;
            for (int j = 0; j < len - i + 1; j++) {
                sub = s.substr(j, i);
                if (valid(s, sub, 0, 0, len - 1, ++count, dp, visited)) p = !p[0] ? sub : min(p, sub);
            }

            if (p[0]) {
                cout << p;
                exit(0);
            }
        }
}
