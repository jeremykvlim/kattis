#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    queue<pair<int, int>> q1;
    vector<vector<bool>> visited1(n, vector<bool>(m, false));
    auto add_border = [&](int r, int c) {
        if (grid[r][c] != '#' || visited1[r][c]) return;
        q1.emplace(r, c);
        visited1[r][c] = true;
    };
    for (int r = 0; r < n; r++) {
        add_border(r, 0);
        add_border(r, m - 1);
    }
    for (int c = 0; c < m; c++) {
        add_border(0, c);
        add_border(n - 1, c);
    }

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    while (!q1.empty()) {
        auto [r, c] = q1.front();
        q1.pop();

        for (int i = 0; i < 4; i++) {
            int x = r + dx[i], y = c + dy[i];
            if (!(0 <= x && x < n && 0 <= y && y < m) || grid[x][y] != '#' || visited1[x][y]) continue;
            q1.emplace(x, y);
            visited1[x][y] = true;
        }
    }

    vector<pair<int, int>> cells;
    vector<vector<int>> indices(n, vector<int>(m, -1));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (grid[r][c] != '#') {
                cells.emplace_back(r, c);
                indices[r][c] = cells.size() - 1;
            }

    int V = cells.size();
    vector<int> dist(V, 1e9), next(V, -1);
    queue<int> q2;
    for (int v = 0; v < V; v++) {
        auto [r, c] = cells[v];
        char ch = grid[r][c];
        int x = r - (ch == '^') + (ch == 'v'), y = c - (ch == '<') + (ch == '>');
        if (0 <= x && x < n && 0 <= y && y < m)
            if (indices[x][y] != -1) next[v] = indices[x][y];

        for (int i = 0; i < 4; i++) {
            x = r + dx[i], y = c + dy[i];
            if (!(0 <= x && x < n && 0 <= y && y < m) || grid[x][y] != '#') continue;
            if (visited1[x][y]) {
                q2.emplace(v);
                dist[v] = 0;
                goto next;
            }
        }
        next:;
    }

    while (!q2.empty()) {
        int v = q2.front();
        q2.pop();

        int u = next[v];
        if (u == -1 || dist[u] != 1e9) continue;
        q2.emplace(u);
        dist[u] = dist[v] + 1;
    }

    if (any_of(dist.begin(), dist.end(), [&](int d) { return d >= s; })) {
        cout << "impossible";
        exit(0);
    }

    vector<int> degree(V, 0);
    for (int u : next)
        if (~u) degree[u]++;

    for (int v = 0; v < V; v++)
        if (!degree[v]) q2.emplace(v);

    int count = 0;
    vector<int> dp1(V, 0);
    while (!q2.empty()) {
        int v = q2.front();
        q2.pop();

        if (!dp1[v]) {
            dp1[v] = s - dist[v];
            count++;
        }

        int u = next[v];
        if (u == -1) continue;
        dp1[u] = max(dp1[u], dp1[v] - 1);
        if (!--degree[u]) q2.emplace(u);
    }

    vector<bool> visited2(V, false);
    for (int v = 0; v < V; v++) {
        if (degree[v] <= 0 || visited2[v]) continue;

        vector<int> cycle;
        for (int u = v;;) {
            visited2[u] = true;
            cycle.emplace_back(u);
            u = next[u];
            if (u == v) break;
        }

        for (int _ = 0; _ < 2; _++)
            for (int u : cycle) {
                int t = next[u];
                dp1[t] = max(dp1[t], dp1[u] - 1);
            }
        if (all_of(cycle.begin(), cycle.end(), [&](int u) { return dp1[u]; })) continue;

        int k = cycle.size();
        vector<int> dp2(k + 1, 0), dp3(k + 1, 0);
        for (int i = k - 1; ~i; i--) {
            int u = cycle[i];
            if (dp1[u]) {
                dp2[i] = dp2[i + 1];
                dp3[i] = dp3[i + 1];
                continue;
            }

            int j = i + s - dist[u];
            if (j >= k) {
                dp2[i] = 1;
                dp3[i] = j - k;
            } else {
                dp2[i] = dp2[j] + 1;
                dp3[i] = dp3[j];
            }
        }

        int c = INT_MAX;
        for (int i = 0; i < k; i++)
            if (dp3[i] >= i) c = min(c, dp2[i]);
        count += c;
    }
    cout << count;
}
