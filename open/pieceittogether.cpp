#include <bits/stdc++.h>
using namespace std;

struct TwoSATSystem {
    int n;
    vector<vector<int>> adj_list;

    TwoSATSystem(int n = 0) : n(n), adj_list(2 * n) {}

    int add_variable() {
        adj_list.emplace_back();
        adj_list.emplace_back();
        return n++;
    }

    void add_clause(int i, int j) {
        i = max(i << 1, -(i << 1 | 1));
        j = max(j << 1, -(j << 1 | 1));
        adj_list[i ^ 1].emplace_back(j);
        adj_list[j ^ 1].emplace_back(i);
    }

    void assign(int i) {
        add_clause(i, i);
    }

    void add_at_most_one(const vector<int> &literals) {
        if (literals.size() < 2) return;

        int prev = ~literals[0];
        for (int i = 2; i < literals.size(); i++) {
            int curr = add_variable();
            add_clause(prev, ~literals[i]);
            add_clause(prev, curr);
            add_clause(~literals[i], curr);
            prev = ~curr;
        }
        add_clause(prev, ~literals[1]);
    }

    vector<int> tarjan() {
        vector<int> order(2 * n, 0), low(2 * n, 0), component(2 * n, 0);
        vector<bool> stacked(2 * n, false);
        stack<int> st;
        int count = 0, sccs = 0;

        auto dfs = [&](auto &&self, int v) -> void {
            order[v] = low[v] = ++count;
            st.emplace(v);
            stacked[v] = true;

            for (int u : adj_list[v])
                if (!order[u]) {
                    self(self, u);
                    low[v] = min(low[v], low[u]);
                } else if (stacked[u]) low[v] = min(low[v], order[u]);

            if (order[v] == low[v]) {
                sccs++;
                int u;
                do {
                    u = st.top();
                    st.pop();
                    stacked[u] = false;
                    component[u] = sccs;
                } while (u != v);
            }
        };

        for (int v = 0; v < 2 * n; v++)
            if (!order[v]) dfs(dfs, v);

        return component;
    }

    pair<bool, vector<int>> solve() {
        vector<int> assignment(n), component = tarjan();
        for (int i = 0; i < n; i++) {
            if (component[i << 1] == component[i << 1 | 1]) return {false, {}};
            assignment[i] = component[i << 1] < component[i << 1 | 1];
        }
        return {true, assignment};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    int b = 0, w = 0;
    for (auto &row : grid)
        for (char c : row) {
            b += c == 'B';
            w += c == 'W';
        }

    if (w != 2 * b) {
        cout << "NO";
        exit(0);
    }

    TwoSATSystem sat;
    vector<vector<int>> hori(n, vector<int>(m, -1)), verti(n, vector<int>(m, -1));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (grid[r][c] == 'B') {
                hori[r][c] = sat.add_variable();
                verti[r][c] = sat.add_variable();
            }

    vector<vector<vector<int>>> literals(n, vector<vector<int>>(m));
    for (int row = 0; row < n; row++)
        for (int col = 0; col < m; col++)
            if (grid[row][col] == 'B') {
                auto white = [&](int r, int c) {
                    return 0 <= r && r < n && 0 <= c && c < m && grid[r][c] == 'W';
                };
                bool u = white(row - 1, col), d = white(row + 1, col), l = white(row, col - 1), r = white(row, col + 1);
                if ((!u && !d) || (!l && !r)) {
                    cout << "NO";
                    exit(0);
                }

                int h = hori[row][col], v = verti[row][col];
                if (!l) sat.assign(h);
                if (!r) sat.assign(~h);
                if (!u) sat.assign(v);
                if (!d) sat.assign(~v);
                if (l) literals[row][col - 1].emplace_back(~h);
                if (r) literals[row][col + 1].emplace_back(h);
                if (u) literals[row - 1][col].emplace_back(~v);
                if (d) literals[row + 1][col].emplace_back(v);
            }

    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (grid[r][c] == 'W') {
                if (literals[r][c].empty()) {
                    cout << "NO";
                    exit(0);
                }

                for (int i = 0; i < literals[r][c].size(); i++)
                    for (int j = i + 1; j < literals[r][c].size(); j++) sat.add_clause(~literals[r][c][i], ~literals[r][c][j]);
            }
    cout << (sat.solve().first ? "YES" : "NO");
}