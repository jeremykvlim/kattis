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

    int n, k, m;
    cin >> n >> k >> m;

    vector<int> p3(k + 1, 1);
    for (int i = 1; i <= k; i++) p3[i] = p3[i - 1] * 3;

    vector<pair<int, int>> edges1, edges2;
    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        if (u <= k && v <= k) edges1.emplace_back(u, v);
        else if (u > k && v > k) edges2.emplace_back(u, v);
        else if (v <= k && u > k) adj_list[u].emplace_back(v);
        else if (u <= k && v > k) adj_list[v].emplace_back(u);
    }

    for (int t = 0; t < p3[k]; t++) {
        vector<int> color1(k + 1, 0);
        for (int i = 1; i <= k; i++) color1[i] = (t / p3[i - 1]) % 3 + 1;

        bool invalid = false;
        for (auto [u, v] : edges1)
            if (color1[u] == color1[v]) {
                invalid = true;
                break;
            }
        if (invalid) continue;

        vector<int> masks(n + 1, 0);
        for (int v = k + 1; v <= n; v++) {
            masks[v] = 7;
            for (int u : adj_list[v]) masks[v] &= ~(1 << (color1[u] - 1));
            if (!masks[v]) {
                invalid = true;
                break;
            }
        }
        if (invalid) continue;

        int count = 0;
        vector<int> color2(n + 1, 0), a(n + 1, 0), b(n + 1, 0), ids(n + 1, -1);
        for (int v = k + 1; v <= n; v++) {
            int p = popcount((unsigned) masks[v]);
            if (p == 1) {
                for (int c = 1; c <= 3; c++)
                    if ((masks[v] >> (c - 1)) & 1) {
                        color2[v] = c;
                        break;
                    }
            } else if (p == 2) {
                int c = 1;
                for (; c <= 3; c++)
                    if ((masks[v] >> (c - 1)) & 1) {
                        a[v] = c++;
                        break;
                    }
                for (; c <= 3; c++)
                    if ((masks[v] >> (c - 1)) & 1) {
                        b[v] = c;
                        break;
                    }
                ids[v] = count++;
            } else {
                invalid = true;
                break;
            }
        }
        if (invalid) continue;

        for (auto [u, v] : edges2)
            if (color2[u] && color2[v] && color2[u] == color2[v]) {
                invalid = true;
                break;
            }
        if (invalid) continue;

        TwoSATSystem sat(count);
        auto id = [&](int v, int c) {
            return a[v] == c ? ids[v] : ~ids[v];
        };
        for (auto [u, v] : edges2) {
            if (color2[u] && !color2[v]) {
                int c = color2[u];
                if ((masks[v] >> (c - 1)) & 1) sat.assign(~id(v, c));
            } else if (!color2[u] && color2[v]) {
                int c = color2[v];
                if ((masks[u] >> (c - 1)) & 1) sat.assign(~id(u, c));
            } else if (!color2[u] && !color2[v]) {
                int overlap = masks[u] & masks[v];
                for (int c = 1; c <= 3; c++)
                    if ((overlap >> (c - 1)) & 1) sat.add_clause(~id(u, c), ~id(v, c));
            }
        }
        auto [satisfiable, assignment] = sat.solve();
        if (satisfiable) {
            for (int u = k + 1; u <= n; u++)
                if (color2[u]) color1[u] = color2[u];
                else color1[u] = assignment[ids[u]] ? a[u] : b[u];
                
            cout << "possible\n";
            for (int i = 1; i <= n; i++) cout << color1[i] << " ";
            exit(0);
        }
    }
    cout << "impossible";
}
