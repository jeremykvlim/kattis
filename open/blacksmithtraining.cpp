#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> e(n + 1, 0), m(n + 1, 301);
    vector<long long> p(n + 1, 0), d(n + 1, 1e10);
    int m_max = -1;
    for (int i = 1; i <= n; i++) {
        cin >> e[i] >> m[i] >> p[i] >> d[i];

        m_max = max(m_max, m[i]);
    }

    vector<vector<vector<bool>>> visited(m_max + 1, vector<vector<bool>>(m_max + 1, vector<bool>(n + 1, false)));
    vector<vector<vector<long long>>> memo(m_max + 1, vector<vector<long long>>(m_max + 1, vector<long long>(n + 1, 1e10)));
    auto dp = [&](auto &&self, int l, int r, int i) -> long long {
        if (l >= r) return 0;
        if (visited[l][r][i]) return memo[l][r][i];
        visited[l][r][i] = true;

        if (e[i] <= l && r <= m[i]) memo[l][r][i] = min(memo[l][r][i], d[i] * (r - l));
        for (int j = 1; j <= n; j++)
            if (d[i] > d[j]) {
                int lm = max(l, e[j]), rm = min(r, m[j]);
                if (lm < rm) memo[l][r][i] = min(memo[l][r][i], p[j] + self(self, l, lm, i) + self(self, lm, rm, j) + self(self, rm, r, i));
            }
        return memo[l][r][i];
    };
    cout << dp(dp, 0, m_max, 0);
}
