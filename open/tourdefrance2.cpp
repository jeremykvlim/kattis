#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<vector<pair<int, int>>> adj_list(n);
        vector<int> indegree(n, 0);
        while (m--) {
            int i, j, d;
            cin >> i >> j >> d;

            adj_list[i].emplace_back(j, d);
            indegree[j]++;
        }

        vector<bool> visited(n);
        int len = 1e9;
        auto dfs = [&](auto &&self, int v = 0, int s = 0, int d = 0) {
            visited[v] = true;

            if (d == n - 1) {
                for (auto [u, w] : adj_list[v])
                    if (!u) len = min(len, s + w);
                visited[v] = false;
                return;
            }

            if (adj_list[v].empty()) {
                visited[v] = false;
                return;
            }

            if (adj_list[v].size() == 1) {
                auto [u, w] = adj_list[v][0];
                if (!visited[u]) self(self, u, s + w, d + 1);
                visited[v] = false;
                return;
            }

            auto [u1, w1] = adj_list[v][0];
            auto [u2, w2] = adj_list[v][1];

            if (visited[u1] || (!visited[u2] && indegree[u2] <= 1)) {
                if (!visited[u2]) {
                    indegree[u1]--;
                    self(self, u2, s + w2, d + 1);
                    indegree[u1]++;
                }
                visited[v] = false;
                return;
            }

            if (visited[u2] || (!visited[u1] && indegree[u1] <= 1)) {
                if (!visited[u1]) {
                    indegree[u2]--;
                    self(self, u1, s + w1, d + 1);
                    indegree[u2]++;
                }
                visited[v] = false;
                return;
            }

            if (!visited[u2]) {
                indegree[u1]--;
                self(self, u2, s + w2, d + 1);
                indegree[u1]++;
            }

            if (!visited[u1]) {
                indegree[u2]--;
                self(self, u1, s + w1, d + 1);
                indegree[u2]++;
            }
            visited[v] = false;
        };
        dfs(dfs);
        cout << len << "\n";
    }
}
