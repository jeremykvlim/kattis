#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

pair<vector<int>, vector<int>> konig(int n, int m, const vector<vector<int>> &adj_list, const vector<int> &l, const vector<int> &r) {
    vector<bool> visited_l(n, false), visited_r(m, false);
    queue<pair<int, bool>> q;
    for (int v = 0; v < n; v++)
        if (!~l[v]) {
            visited_l[v] = true;
            q.emplace(v, true);
        }

    while (!q.empty()) {
        auto [v, left] = q.front();
        q.pop();

        if (left) {
            for (int u : adj_list[v])
                if (!visited_r[u] && l[v] != u) {
                    visited_r[u] = true;
                    q.emplace(u, false);
                }
        } else {
            int u = r[v];
            if (u != -1 && !visited_l[u]) {
                visited_l[u] = true;
                q.emplace(u, true);
            }
        }
    }

    vector<int> cover_l, cover_r;
    for (int v = 0; v < n; v++)
        if (!visited_l[v]) cover_l.emplace_back(v);
    for (int v = 0; v < m; v++)
        if (visited_r[v]) cover_r.emplace_back(v);

    return {cover_l, cover_r};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    vector<pair<int, int>> edges1(m);
    vector<vector<int>> adj_list1(1e3);
    for (auto &[i, j] : edges1) {
        cin >> i >> j;
        i -= 1e3;
        j -= 2e3;

        adj_list1[i].emplace_back(j);
    }
    auto [l1, r1, _] = hopcroft_karp(1e3, 1e3, edges1);
    auto [cover_l1, cover_r1] = konig(1e3, 1e3, adj_list1, l1, r1);

    vector<pair<int, int>> edges2;
    vector<vector<int>> adj_list2(1e3);
    for (auto [u, v] : edges1)
        if (u != 9) {
            edges2.emplace_back(u, v);
            adj_list2[u].emplace_back(v);
        }
    auto [l2, r2, __] = hopcroft_karp(1e3, 1e3, edges2);
    auto [cover_l2, cover_r2] = konig(1e3, 1e3, adj_list2, l2, r2);

    vector<int> employees;
    if (cover_l1.size() + cover_r1.size() == cover_l2.size() + cover_r2.size() + 1) {
        employees.emplace_back(1009);
        for (int u : cover_l2)
            if (u != 9) employees.emplace_back(1e3 + u);
        for (int v : cover_r2) employees.emplace_back(2e3 + v);
    } else {
        for (int u : cover_l1) employees.emplace_back(1e3 + u);
        for (int v : cover_r1) employees.emplace_back(2e3 + v);
    }

    cout << employees.size() << "\n";
    for (int id : employees) cout << id << "\n";
}
