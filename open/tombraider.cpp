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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    int cells = n * m;
    DisjointSets dsu(4 * cells);

    auto side = [&](int r, int c, int d) {
        return 4 * (r * m + c) + d;
    };

    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++) {
            if (r + 1 < n) dsu.unite(side(r, c, 2), side(r + 1, c, 0));
            if (c + 1 < m) dsu.unite(side(r, c, 1), side(r, c + 1, 3));

            if (grid[r][c] == '.') {
                dsu.unite(side(r, c, 0), side(r, c, 2));
                dsu.unite(side(r, c, 1), side(r, c, 3));
            } else if (grid[r][c] == '/') {
                dsu.unite(side(r, c, 0), side(r, c, 3));
                dsu.unite(side(r, c, 1), side(r, c, 2));
            } else if (grid[r][c] == '\\') {
                dsu.unite(side(r, c, 0), side(r, c, 1));
                dsu.unite(side(r, c, 2), side(r, c, 3));
            }
        }

    vector<char> face;
    vector<vector<int>> indices(n, vector<int>(m, -1));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (grid[r][c] == 'V' || grid[r][c] == 'H') {
                face.emplace_back(grid[r][c]);
                indices[r][c] = face.size() - 1;
            }

    if (face.empty()) {
        cout << 0;
        exit(0);
    }

    vector<bool> obstacle(4 * cells, false);
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (grid[r][c] == '#')
                for (int d = 0; d < 4; d++) obstacle[dsu.find(side(r, c, d))] = true;

    vector<int> literals(4 * cells, INT_MIN), blocked;
    TwoSATSystem sat(face.size());
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++)
            if (~indices[r][c]) {
                int i = indices[r][c];
                for (int d = 0; d < 4; d++) {
                    int v = dsu.find(side(r, c, d)), literal = !(d & 1) ? i : ~i;
                    if (obstacle[v]) {
                        sat.assign(~literal);
                        blocked.emplace_back(literal);
                    } else if (literals[v] == INT_MIN) literals[v] = literal;
                    else {
                        sat.add_clause(~literals[v], literal);
                        sat.add_clause(~literal, literals[v]);
                    }
                }
            }

    auto component = sat.tarjan();
    for (int i = 0; i < sat.n; i++)
        if (component[i << 1] == component[i << 1 | 1]) {
            cout << -1;
            exit(0);
        }

    int sccs = *max_element(component.begin(), component.end());
    vector<int> opposite(sccs + 1), count(sccs + 1);
    for (int i = 0; i < face.size(); i++) {
        int verti = component[i << 1], hori = component[i << 1 | 1];
        opposite[verti] = hori;
        opposite[hori] = verti;
        count[verti] += face[i] == 'H';
        count[hori] += face[i] == 'V';
    }

    vector<bool> invalid(sccs + 1, false);
    for (int literal : blocked) invalid[component[literal >= 0 ? literal << 1 : ((~literal) << 1 | 1)]] = true;

    int gargoyles = 0;
    vector<bool> visited(sccs + 1, false);
    for (int c = 1; c <= sccs; c++)
        if (!visited[c]) {
            int o = opposite[c];
            visited[c] = visited[o] = true;
            if (invalid[c] && invalid[o]) {
                cout << -1;
                exit(0);
            }

            if (invalid[c]) gargoyles += count[o];
            else if (invalid[o]) gargoyles += count[c];
            else gargoyles += min(count[c], count[o]);
        }
    cout << gargoyles;
}