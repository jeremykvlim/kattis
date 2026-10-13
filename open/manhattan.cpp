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

    int t;
    cin >> t;

    while (t--) {
        int S, A, m;
        cin >> S >> A >> m;

        TwoSATSystem sat(S + A);
        auto street = [&](int s, int a1, int a2) {
            return a1 < a2 ? s : ~s;
        };

        auto avenue = [&](int a, int s1, int s2) {
            return s1 < s2 ? a + S : ~(a + S);
        };

        while (m--) {
            int s1, a1, s2, a2;
            cin >> s1 >> a1 >> s2 >> a2;
            s1--;
            a1--;
            s2--;
            a2--;

            if (s1 == s2 && a1 == a2) continue;

            if (s1 == s2) {
                sat.assign(street(s1, a1, a2));
                continue;
            }

            if (a1 == a2) {
                sat.assign(avenue(a1, s1, s2));
                continue;
            }

            int u = street(s1, a1, a2), v = street(s2, a1, a2), x = avenue(a1, s1, s2), y = avenue(a2, s1, s2);
            sat.add_clause(u, x);
            sat.add_clause(u, v);
            sat.add_clause(y, x);
            sat.add_clause(y, v);
        }
        cout << (sat.solve().first ? "Yes\n" : "No\n");
    }
}