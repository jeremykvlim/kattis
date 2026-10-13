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

    vector<int> d(7), pref(8, 0);
    for (int i = 0; i < 7; i++) {
        cin >> d[i];

        pref[i + 1] = pref[i] + d[i];
    }

    auto literal = [&](int day, int b) {
        int v = pref[day] + abs(b) - 1;
        return b > 0 ? v : ~v;
    };

    vector<vector<int>> friends(n, vector<int>(7));
    for (auto &literals : friends)
        for (int day = 0; day < 7; day++) {
            int b;
            cin >> b;

            literals[day] = literal(day, b);
        }

    int b6, b7;
    cin >> b6 >> b7;

    TwoSATSystem sat(pref[7]);
    int v6 = literal(5, b6), v7 = literal(6, b7);
    sat.add_clause(v6, v7);
    for (auto &literals : friends) {
        sat.add_at_most_one(literals);
        for (int u : literals) {
            sat.add_clause(~u, v6);
            sat.add_clause(~u, v7);
        }
    }
    cout << (sat.solve().first ? "possible" : "impossible");
}