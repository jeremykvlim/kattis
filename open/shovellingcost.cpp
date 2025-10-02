#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, vector<int>> dreyfus_wagner(int n, const vector<array<int, 3>> &edges, const vector<int> &terminals, const vector<T> &cost) {
    vector<vector<array<int, 3>>> adj_list(n);
    for (int i = 0; i < edges.size(); i++) {
        auto [u, v, w] = edges[i];
        adj_list[u].push_back({v, w, i});
    }

    int t = terminals.size();
    T inf = numeric_limits<T>::max() >> 2;
    vector<vector<T>> dp(1 << t, vector<T>(n, inf));
    for (int i = 0; i < t; i++) dp[1 << i][terminals[i]] = 0;
    vector<vector<pair<int, int>>> prev(1 << t, vector<pair<int, int>>(n, {-1, -1}));

    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
    for (int m1 = 1; m1 < 1 << t; m1++) {
        for (int v = 0; v < n; v++)
            for (int m2 = (m1 - 1) & m1; m2; --m2 &= m1) {
                T d = dp[m2][v] + dp[m1 ^ m2][v] - cost[v];
                if (dp[m1][v] > d) {
                    dp[m1][v] = d;
                    prev[m1][v] = {m2, -1};
                }
            }

        auto &dist = dp[m1];
        for (int v = 0; v < n; v++) pq.emplace(dist[v], v);

        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w, i] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                    prev[m1][u] = {v, i};
                }
        }
    }

    T total = inf;
    int v = -1;
    for (int u = 0; u < n; u++) {
        T d = dp.back()[u] + cost[u];
        if (total > d) {
            total = d;
            v = u;
        }
    }

    vector<vector<bool>> visited(1 << t, vector<bool>(n, false));
    vector<int> steiner;
    auto dfs = [&](auto &&self, int m1, int v) -> void {
        if (visited[m1][v]) return;

        visited[m1][v] = true;
        steiner.emplace_back(v);

        auto [m2, i] = prev[m1][v];
        if (!~m2) return;
        if (!~i) {
            self(self, m2, v);
            self(self, m1 ^ m2, v);
        } else self(self, m1, m2);
    };
    dfs(dfs, (1 << t) - 1, v);
    return {total, steiner};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n && m) {
        vector<string> grid(m);
        for (auto &row : grid) cin >> row;

        auto index = [&](int i, int j) {
            return i * n + j;
        };

        vector<int> cost(n * m, 1e6), terminals;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '#') continue;

                int v = index(i, j);
                cost[v] = grid[i][j] == 'o';
                if (isupper(grid[i][j])) terminals.emplace_back(v);
            }

        vector<array<int, 3>> edges;
        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '#') continue;
                
                int u = index(i, j);
                for (int k = 0; k < 4; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    if (!(0 <= x && x < m && 0 <= y && y < n) || grid[x][y] == '#') continue;
                    int v = index(x, y);
                    edges.push_back({u, v, cost[v]});
                }
            }

        auto [total, steiner] = dreyfus_wagner(n * m, edges, terminals, cost);
        cout << total << "\n";
    }
}
