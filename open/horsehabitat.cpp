#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    vector<vector<int>> pref_height(n + 2, vector<int>(m + 2, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (grid[i - 1][j - 1] == '.') pref_height[i][j] = pref_height[i - 1][j] + 1;

    vector<vector<int>> count(n + 2, vector<int>(m + 2, 0));
    for (int i = 1; i <= n; i++) {
        vector<pair<int, int>> s{{0, 1}};
        for (int j = 0; j < m + 2; j++) {
            int h = pref_height[i][j], w = 0;
            while (s.size() > 1 && s.back().first >= h) {
                w += s.back().second;
                count[max(s[s.size() - 2].first, h) + 1][w]++;
                count[s.back().first + 1][w]--;
                s.pop_back();
            }
            s.emplace_back(h, w + 1);
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) count[i + 1][j] += count[i][j];

        for (int _ = 0; _ < 2; _++)
            for (int j = m; j; j--) count[i][j] += count[i][j + 1];
    }

    while (q--) {
        int h, w;
        cin >> h >> w;
        cout << count[h][w] << "\n";
    }
}
