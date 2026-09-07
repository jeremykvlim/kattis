#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    int components = 0;
    vector<vector<int>> component(n, vector<int>(m, -1));
    for (int row = 0; row < n; row++)
        for (int col = 0; col < m; col++)
            if (grid[row][col] == '?' && !~component[row][col]) {
                component[row][col] = components;
                queue<pair<int, int>> q;
                q.emplace(row, col);
                while (!q.empty()) {
                    auto [r, c] = q.front();
                    q.pop();

                    for (int dr = -1; dr <= 1; dr++)
                        for (int dc = -1; dc <= 1; dc++) {
                            if (!dr && !dc) continue;

                            int x = r + dr, y = c + dc;
                            if (0 <= x && x < n && 0 <= y && y < m && grid[x][y] == '?' && !~component[x][y]) {
                                q.emplace(x, y);
                                component[x][y] = components;
                            }
                        }
                }
                components++;
            }

    vector<vector<int>> adj_list(components);
    vector<int> state(components, -1);
    for (int row = 0; row < n; row++)
        for (int col = 0; col < m; col++)
            if (isdigit(grid[row][col])) {
                vector<pair<int, int>> runs;
                for (int dr = -1; dr <= 1; dr++)
                    for (int dc = -1; dc <= 1; dc++) {
                        if (!dr && !dc) continue;

                        int x = row + dr, y = col + dc;
                        if (0 <= x && x < n && 0 <= y && y < m && grid[x][y] == '?') {
                            int c = component[x][y], i = 0;
                            for (; i < runs.size() && runs[i].first != c; i++);

                            if (i == runs.size()) runs.emplace_back(c, 1);
                            else runs[i].second++;
                        }
                    }

                auto update = [&](int c, int s) {
                    if (!~state[c]) state[c] = s;
                };

                if (runs.size() == 1) update(runs[0].first, 1);
                else if (runs.size() == 2) {
                    auto [c1, len1] = runs[0];
                    auto [c2, len2] = runs[1];

                    int d = grid[row][col] - '0';
                    if (len1 == d && len2 == d) {
                        adj_list[c1].emplace_back(c2);
                        adj_list[c2].emplace_back(c1);
                    } else if (len1 + len2 == d) {
                        update(c1, 1);
                        update(c2, 1);
                    } else if (len1 == d) {
                        update(c1, 1);
                        update(c2, 0);
                    } else {
                        update(c1, 0);
                        update(c2, 1);
                    }
                }
            }

    vector<int> color(components, -1);
    vector<bool> possible(components, false);
    for (int c = 0; c < components; c++)
        if (!~color[c]) {
            int b = -1;
            vector<int> order;
            auto dfs = [&](auto &&self, int v) -> void {
                order.emplace_back(v);
                if (~state[v]) b = state[v] ^ color[v];

                for (int u : adj_list[v])
                    if (!~color[u]) {
                        color[u] = color[v] ^ 1;
                        self(self, u);
                    }
            };
            color[c] = 0;
            dfs(dfs, c);
            for (int v : order) possible[v] = !~b ? true : color[v] ^ b;
        }

    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (grid[r][c] == '?') grid[r][c] = possible[component[r][c]] ? 'F' : 'B';

    for (auto &row : grid) cout << row << "\n";
}
