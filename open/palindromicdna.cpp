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
        for (int i = 2; i < (int)literals.size(); i++) {
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
            for (int i = 0; i < (int)q.size(); i++) {
                int v = q[i];
                for (int u : adj_list[v]) {
                    if (assignment[u >> 1] == -1) {
                        assignment[u >> 1] = !(u & 1);
                        q.emplace_back(u);
                    } else if (assignment[u >> 1] == (u & 1)) return false;
                }
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

    int n, t;
    while (cin >> n >> t && n) {
        string s;
        cin >> s;

        auto index = [](char c) {
            switch (c) {
                case 'A': return 0;
                case 'G': return 1;
                case 'T': return 2;
                case 'C': return 3;
            }
        };

        vector<int> dna(n);
        for (int i = 0; i < n; i++) dna[i] = index(s[i]);

        vector<int> a(n), b(n);
        iota(a.begin(), a.end(), 0);
        iota(b.begin(), b.end(), n);
        TwoSATSystem sat(2 * n);
        for (int i = 0; i + 1 < n; i++) sat.add_clause(~a[i], ~a[i + 1]);
        for (int i = 0; i < n; i++) sat.add_clause(~b[i], a[i]);

        while (t--) {
            int l;
            char colon;
            cin >> l >> colon;

            vector<int> pos(l);
            for (int &p : pos) cin >> p;

            for (int k = 0; k < l / 2; k++) {
                int i = pos[k], j = pos[l - 1 - k];
                if (i != j) {
                    int diff = (dna[i] - dna[j] + 4) % 4;
                    if (!diff) {
                        sat.add_clause(a[i], ~a[j]);
                        sat.add_clause(~a[i], a[j]);
                        sat.add_clause(b[i], ~b[j]);
                        sat.add_clause(~b[i], b[j]);
                    } else if (diff != 2) {
                        sat.add_clause(a[i], a[j]);
                        sat.add_clause(~a[i], ~a[j]);
                        if (diff == 3) {
                            sat.add_clause(~a[i], ~b[i]);
                            sat.add_clause(~a[j], b[j]);
                        } else {
                            sat.add_clause(~a[i], b[i]);
                            sat.add_clause(~a[j], ~b[j]);
                        }
                    } else {
                        sat.assign(a[i]);
                        sat.assign(a[j]);
                        sat.add_clause(b[i], b[j]);
                        sat.add_clause(~b[i], ~b[j]);
                    }
                }
            }
        }
        cout << (sat.solve().first ? "YES\n" : "NO\n");
    }
}
