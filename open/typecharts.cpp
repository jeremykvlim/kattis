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

    pair<bool, vector<int>> solve() {
        deque<int> q;
        vector<int> assignment(n, -1);
        auto bfs = [&](int s) {
            q = {s};
            assignment[s >> 1] = !(s & 1);
            for (int i = 0; i < q.size(); i++) {
                int v = q[i];
                for (int u : adj_list[v])
                    if (assignment[u >> 1] == -1) {
                        assignment[u >> 1] = !(u & 1);
                        q.emplace_back(u);
                    } else if (assignment[u >> 1] == (u & 1)) return false;
            }
            return true;
        };

        for (int i = 0; i < n; i++) {
            if (assignment[i] != -1 || bfs(i << 1 | 1)) continue;
            for (int v : q) assignment[v >> 1] = -1;
            if (!bfs(i << 1)) return {false, {}};
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