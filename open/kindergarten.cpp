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

    int n;
    cin >> n;

    vector<int> teacher(n);
    vector<vector<int>> rank(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        cin >> teacher[i];

        for (int j = 1; j < n; j++) {
            int k;
            cin >> k;

            rank[i][k - 1] = j;
        }
    }

    int l = -1, r = n - 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        auto choice = [&](int i, int t) {
            return t == (teacher[i] + 1) % 3 ? i : ~i;
        };

        TwoSATSystem sat(n);
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (rank[i][j] > m || rank[j][i] > m)
                    for (int t = 0; t < 3; t++)
                        if (t != teacher[i] && t != teacher[j]) sat.add_clause(~choice(i, t), ~choice(j, t));
        
        if (sat.solve().first) r = m;
        else l = m;
    }
    cout << r;
}