#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> adj_list(n + 1);
    vector<vector<bool>> adj_matrix(n + 1, vector<bool>(n + 1, false));
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
        adj_matrix[a][b] = adj_matrix[b][a] = true;
    }

    vector<int> seq(n + 1);
    vector<bool> visited(n + 1, false);
    int flights = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (visited[j]) continue;

            vector<pair<int, int>> dp(n + 1, {0, 0});
            vector<int> degree(n + 1, 0);
            int even = 0, odd = 0, edges = 0;
            auto dfs = [&](auto &&self, int v, int prev = -1, int depth = 0) -> void {
                if (!(depth & 1)) even++;
                else odd++;

                if (~prev) edges++;
                int sum = 0, min1 = 1e6, min2 = 1e5;
                for (int u : adj_list[v]) {
                    if (u != prev && !visited[u]) {
                        degree[v]++;
                        degree[u]++;
                        self(self, u, v, depth + 1);

                        sum += dp[u].second;
                        int d = dp[u].first - dp[u].second;
                        if (min1 >= d) {
                            min2 = min1;
                            min1 = d;
                        } else if (min2 > d) min2 = d;
                    }
                }

                dp[v].first = min(sum + 1, sum + min1);
                dp[v].second = min(dp[v].first, sum + min1 + min2 - 1);
            };
            dfs(dfs, j);

            int l = dp[j].first;
            for (int v = 1; v <= n; v++)
                if (!visited[v] && !dp[v].first) {
                    dfs(dfs, v);
                    l += dp[v].second;
                }
            int r = even + odd;
            if (r > 1 && edges == r - 1 && even * odd == r - 1 || r > 2 && edges - degree[j] == r - 2 && (even - 1) * odd == r - 2) r--;

            if (l - 1 + flights - adj_matrix[seq[i - 1]][j] <= k && k < r + flights - adj_matrix[seq[i - 1]][j]) {
                flights += !adj_matrix[seq[i - 1]][j];
                visited[j] = true;
                seq[i] = j;
                break;
            }
        }

        if (!seq[i]) {
            cout << -1;
            exit(0);
        }
    }
    for (int i = 1; i <= n; i++) cout << seq[i] << " ";
}
