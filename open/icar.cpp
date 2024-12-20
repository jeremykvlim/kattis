#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> t(n), g(n), r(n), cycle(n);
    vector<vector<int>> change_times(n);
    for (int i = 1; i < n; i++) {
        cin >> t[i] >> g[i] >> r[i];

        cycle[i] = g[i] + r[i];
        for (int curr = t[i] ? t[i] : cycle[i]; curr < 1e3; curr += cycle[i]) change_times[i].emplace_back(curr);
    }

    vector<pair<double, int>> nodes{{0, 0}};
    for (int i = 1; i <= 20 * n; i++) nodes.emplace_back(i * 50, !(i % 20) && i < 20 * n ? i / 20 : 0);

    int m = nodes.size();
    vector<double> dp(m);
    vector<vector<double>> adj_matrix(m, vector<double>(m, 0));
    vector<int> indices(n), prev(m);
    for (int _ = 0; _ <= n + 1; _++) {
        if (m < nodes.size()) {
            m = nodes.size();
            dp.resize(m);
            adj_matrix.resize(m);
            prev.resize(m);
            for (auto &row : adj_matrix) row.resize(m, 0);
        }

        for (int i = 0; i < m; i++) {
            auto [di, light] = nodes[i];
            if (light) indices[light] = i;
            for (int j = i + 1; j < m; j++) {
                auto [dj, __] = nodes[j];
                adj_matrix[i][j] = sqrt((dj - di) * 2);
            }
        }

        fill(dp.begin(), dp.end(), 1e3);
        dp[0] = 0;
        for (int i = 0; i < m; i++) {
            auto drive = [&](double curr) {
                for (int j = i + 1; j < m; j++) {
                    auto reach = curr + adj_matrix[i][j];

                    auto update = [&]() {
                        if (dp[j] > reach) {
                            dp[j] = reach;
                            prev[j] = i;
                        }
                    };

                    auto [__, light] = nodes[j];
                    if (light) {
                        auto remaining = t[light] - reach + floor(reach / cycle[light]) * cycle[light];
                        if (r[light] <= remaining || (-g[light] <= remaining && remaining <= 0)) update();
                        else {
                            if (remaining <= 0) remaining += cycle[light];
                            reach += remaining;
                            update();
                            break;
                        }
                    } else update();
                }
            };
            drive(dp[i]);
            for (int light = 1; light < n; light++) {
                int j = indices[light];
                if (j > i)
                    for (int time : change_times[light])
                        if (dp[i] < time - adj_matrix[i][j]) drive(time - adj_matrix[i][j]);
            }
        }

        vector<bool> visited(m, false);
        visited[0] = true;
        for (int i = 1; i < n; i++) visited[indices[i]] = true;
        for (int i = m - 1; i; i = prev[i]) visited[i] = true;

        vector<pair<double, int>> temp{{nodes[0]}};
        for (int i = 1; i < m; i++)
            if (visited[i - 1] || visited[i]) {
                auto dist = nodes[i - 1].first, d = (nodes[i].first - dist) / 10;
                for (int __ = 0; __ < 9; __++) {
                    dist += d;
                    temp.emplace_back(dist, 0);
                }
                if (visited[i]) temp.emplace_back(nodes[i]);
            }
        nodes = temp;
    }
    cout << fixed << setprecision(6) << dp[m - 1];
}
