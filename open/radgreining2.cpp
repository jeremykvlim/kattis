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

    vector<pair<int, string>> sections(m);
    for (auto &[s, t] : sections) {
        cin >> s >> t;
        s--;
    }

    TwoSATSystem sat(m);
    for (int i = 1; i < m; i++)
        for (int j = 0; j < i; j++)
            for (int a = 0; a < 2; a++)
                for (int b = 0; b < 2; b++) {
                    auto [s1, t1] = sections[i];
                    auto [s2, t2] = sections[j];
                    for (int k = max(s1, s2); k < min(s1 + t1.size(), s2 + t2.size()); k++)
                        if ((a ? t1[k - s1] : t1[s1 - k + t1.size() - 1]) != (b ? t2[k - s2] : t2[s2 - k + t2.size() - 1])) {
                            sat.add_clause(!a ? i : ~i, !b ? j : ~j);
                            goto next;
                        }
                    next:;
                }


    auto [satisfiable, assignment] = sat.solve();
    if (!satisfiable) cout << "Villa";
    else {
        string virus(n, 'A');
        for (int i = 0; i < m; i++) {
            auto [s, t] = sections[i];
            for (int j = 0; j < t.size(); j++) virus[s + (assignment[i] ? j : t.size() - 1 - j)] = t[j];
        }
        cout << virus;
    }
}
