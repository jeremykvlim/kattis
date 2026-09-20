#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        int m = 2 * n;
        vector<array<int, 3>> dolls(m);
        for (auto &[h, d, w] : dolls) cin >> h >> d >> w;

        auto valid = [&](int i, int j) {
            auto [hi, di, wi] = dolls[i];
            auto [hj, dj, wj] = dolls[j];
            return hi <= hj - 2 * wj && di <= dj - 2 * wj;
        };

        vector<vector<int>> adj_list(m);
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++)
                if (!valid(i, j) && !valid(j, i)) {
                    adj_list[i].emplace_back(j);
                    adj_list[j].emplace_back(i);
                }

        vector<int> color(m, -1);
        vector<array<vector<int>, 2>> components;
        for (int s = 0; s < m; s++) {
            if (!~color[s]) {
                color[s] = 0;
                array<vector<int>, 2> component;
                queue<int> q;
                q.emplace(s);
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();

                    component[color[v]].emplace_back(v);

                    for (int u : adj_list[v])
                        if (!~color[u]) {
                            color[u] = color[v] ^ 1;
                            q.emplace(u);
                        }
                }
                components.emplace_back(component);
            }
        }

        int c = components.size();
        vector<vector<int>> dp(c + 1, vector<int>(n + 1, -1)), parity(c + 1, vector<int>(n + 1, -1));
        dp[0][0] = 0;
        for (int i = 0; i < c; i++) {
            int a = components[i][0].size(), b = components[i][1].size();
            for (int d = 0; d <= n; d++) {
                if (!~dp[i][d]) continue;
                if (d + a <= n && !~dp[i + 1][d + a]) {
                    dp[i + 1][d + a] = d;
                    parity[i + 1][d + a] = 0;
                }
                if (d + b <= n && !~dp[i + 1][d + b]) {
                    dp[i + 1][d + b] = d;
                    parity[i + 1][d + b] = 1;
                }
            }
        }

        array<vector<int>, 2> indices;
        for (int i = c, d = n; i; i--) {
            int p = parity[i][d];
            for (int v : components[i - 1][p]) indices[0].emplace_back(v);
            for (int v : components[i - 1][p ^ 1]) indices[1].emplace_back(v);
            d = dp[i][d];
        }
        for (int k = 0; k < 2; k++) sort(indices[k].begin(), indices[k].end(), [&](int i, int j) { return dolls[i][0] > dolls[j][0]; });

        for (int i : indices[0]) {
            auto [h, d, w] = dolls[i];
            cout << h << " " << d << " " << w << "\n";
        }
        cout << "-\n";
        for (int i : indices[1]) {
            auto [h, d, w] = dolls[i];
            cout << h << " " << d << " " << w << "\n";
        }
        cout << "\n";
    }
}