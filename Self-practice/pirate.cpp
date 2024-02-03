#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, m, n;
    cin >> a >> b >> m >> n;

    if (a > b) swap(a, b);
    vector<vector<int>> d(m, vector<int>(n, 0));
    for (auto &row: d)
        for (int &dij: row) cin >> dij;

    auto volume = 0LL;
    for (int r = 0; r < m; r++) {
        vector<int> d_min(n, INT_MAX);
        for (int i = r; i < m; i++) {
            int w = i - r + 1;
            if (w > b) break;

            stack<pair<long long, int>> s;
            s.emplace(-1, -1);
            for (int j = 0; j <= n; j++) {
                if (j < n) d_min[j] = min(d_min[j], d[i][j]);
                int c = j;
                long long curr = j == n ? -1 : d_min[j], prev = s.top().first;
                for (; prev > curr; s.pop(), prev = s.top().first) {
                    c = s.top().second;
                    long long area = w * min(j - c, w > a ? a : b);
                    volume = max(volume, (m * n * prev - 1) / (m * n - area) * area);
                }

                if (prev < curr) s.emplace(curr, c);
            }
        }
    }

    cout << volume;
}
