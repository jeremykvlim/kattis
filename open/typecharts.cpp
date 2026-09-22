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

    auto index = [&](int i) {
        return 2 * i;
    };

    TwoSATSystem sat(2 * n);
    while (m--) {
        int i, j;
        char c;
        cin >> i >> j >> c;
        i--;
        j--;

        int ui = index(i), vi = index(i) + 1, uj = index(j), vj = index(j) + 1;
        if (i == j) {
            if (c == 'x') {
                sat.assign(~ui);
                sat.assign(~vi);
            } else if (c == '-') {
                sat.assign(~ui);
                sat.assign(vi);
            } else if (c == '=') {
                sat.assign(ui);
                sat.assign(vi);
            } else {
                sat.assign(ui);
                sat.assign(~vi);
            }
            continue;
        }

        if (c == 'x') {
            sat.add_clause(~ui, ~uj);
            sat.add_clause(~ui, ~vj);
            sat.add_clause(~vi, ~uj);
            sat.add_clause(~vi, ~vj);
        } else if (c == '-') {
            sat.assign(vi);
            sat.assign(vj);
            sat.add_clause(~ui, ~uj);
        } else if (c == '=') {
            sat.add_clause(~ui, vj);
            sat.add_clause(ui, ~vj);
            sat.add_clause(~vi, uj);
            sat.add_clause(vi, ~uj);
            sat.add_clause(ui, vi);
        } else {
            sat.assign(ui);
            sat.assign(uj);
            sat.add_clause(~vi, ~vj);
        }
    }

    auto assignment = sat.solve().second;
    for (int i = 0; i < n; i++) {
        int a = assignment[index(i)], b = assignment[index(i) + 1];
        if (!a && !b) cout << 'x';
        else if (!a) cout << '-';
        else if (b) cout << '=';
        else cout << '+';
    }
}