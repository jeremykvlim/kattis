#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<int> m(n + 1);
        for (int i = 1; i <= n; i++) cin >> m[i];

        string s;
        cin >> s;
        int k = s.size();

        vector<int> limit(n + 1);
        limit[n] = m[n];
        for (int i = n - 1; i; i--) limit[i] = m[i] * *max_element(limit.begin() + i + 1, limit.end());

        if (limit[1] < k) {
            cout << "No\n";
            continue;
        }

        vector<vector<int>> lcp(k + 1, vector<int>(k + 1, 0));
        for (int i = k - 1; ~i; i--)
            for (int j = k - 1; ~j; j--)
                if (s[i] == s[j]) lcp[i][j] = lcp[i + 1][j + 1] + 1;

        vector<int> pos(n + 1, 0), len(n + 1, 0);
        len[1] = k;
        auto dfs = [&](auto &&self, int i = 1, int curr_len = 0, int used = 0) -> bool {
            if (curr_len == len[i]) {
                do i++;
                while (i <= n && !len[i]);
                return i > n || self(self, i);
            }
            if (used == m[i]) return false;

            int longest = 1;
            for (int j = i + 1; j <= n; j++) longest = max(longest, len[j] ? len[j] : limit[j]);
            if (curr_len + (m[i] - used) * longest < len[i]) return false;

            int p = pos[i] + curr_len;
            for (int j = i + 1; j <= n; j++)
                if (len[j] && curr_len + len[j] <= len[i] && lcp[p][pos[j]] >= len[j] && self(self, i, curr_len + len[j], used + 1)) return true;

            for (int j = i + 1; j <= n; j++)
                if (!len[j])
                    for (int l = min(limit[j], len[i] - curr_len); l; l--) {
                        pos[j] = p;
                        len[j] = l;
                        if (self(self, i, curr_len + l, used + 1)) return true;
                        len[j] = 0;
                    }
            return self(self, i, curr_len + 1, used + 1);
        };
        cout << (dfs(dfs) ? "Yes\n" : "No\n");
    }
}