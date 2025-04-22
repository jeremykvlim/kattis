#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> h(n, vector<int>(m));
    for (auto &row : h)
        for (int &hij : row) cin >> hij;

    int b;
    cin >> b;

    vector<vector<int>> boost(n, vector<int>(m, 0));
    vector<int> dr{1, 0, -1, 0, 1, -1, -1, 1}, dc{0, 1, 0, -1, 1, 1, -1, -1};
    while (b--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;

        for (int k = 0; k < 8; k++)
            if (dr[k] == r2 - r1 && dc[k] == c2 - c1) {
                boost[r1][c1] |= 1 << k;
                break;
            }
    }

    vector<vector<vector<pair<double, int>>>> dp(n, vector<vector<pair<double, int>>>(m));
    priority_queue<pair<pair<double, int>, pair<int, int>>, vector<pair<pair<double, int>, pair<int, int>>>, greater<>> pq;
    pq.emplace(make_pair(0, 0), make_pair(0, 0));
    while (!pq.empty()) {
        auto [d, s] = pq.top();
        pq.pop();

        auto [t0, v0] = d;
        auto [r0, c0] = s;

        if (r0 == n - 1 && c0 == m - 1) {
            cout << fixed << setprecision(4) << t0;
            exit(0);
        }

        if (any_of(dp[r0][c0].begin(), dp[r0][c0].end(), [&](auto p) {return p.first <= t0 && p.second >= v0;})) continue;

        for (auto it = dp[r0][c0].begin(); it != dp[r0][c0].end();) {
            if (it->first >= t0 && it->second <= v0) it = dp[r0][c0].erase(it);
            else it++;
        }
        dp[r0][c0].emplace_back(t0, v0);
        for (int k = 0; k < 8; k++) {
            int r = r0 + dr[k], c = c0 + dc[k];
            if (!(0 <= r && r < n && 0 <= c && c < m) || v0 <= h[r][c] - h[r0][c0]) continue;

            int v = v0 + h[r0][c0] - h[r][c];
            if ((boost[r0][c0] >> k) & 1) v *= 2;
            v = min(v, 2000);

            pq.emplace(make_pair(t0 + (k < 4 ? 100. : 150.) / v, v), make_pair(r, c));
        }
    }
}
