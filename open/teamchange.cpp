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
    for (int u = 0; u < n; u++)
        if (l[u] == -1) {
            visited_l[u] = true;
            q.emplace(u, true);
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

    int n, R;
    string curr, req;
    cin >> n >> R >> curr >> req;

    vector<pair<int, int>> rivalries(R);
    for (auto &[a, b] : rivalries) {
        cin >> a >> b;
        a--;
        b--;
    }

    vector<bool> diff(n, false), same(n, false);
    for (int i = 0; i < n; i++) {
        if (req[i] != '?' && curr[i] != req[i]) diff[i] = true;
        if (req[i] != '?' && curr[i] == req[i]) same[i] = true;
    }

    vector<vector<int>> adj_list(n);
    vector<pair<int, int>> edges;
    for (auto [a, b] : rivalries) {
        if (!diff[a] && !same[b]) {
            edges.emplace_back(a, b);
            adj_list[a].emplace_back(b);
        }
        if (!diff[b] && !same[a]) {
            edges.emplace_back(b, a);
            adj_list[b].emplace_back(a);
        }
    }

    for (int i = 0; i < n; i++)
        if (!diff[i] && !same[i]) {
            edges.emplace_back(i, i);
            adj_list[i].emplace_back(i);
        }
    auto [l, r, matches] = hopcroft_karp(n, n, edges);
    auto [cover_l, cover_r] = konig(n, n, adj_list, l, r);

    vector<bool> cl(n, false), cr(n, false);
    for (int v : cover_l) cl[v] = true;
    for (int v : cover_r) cr[v] = true;

    string teams(n, 'X');
    for (int i = 0; i < n; i++) {
        if (diff[i]) cl[i] = true;
        if (same[i]) cr[i] = true;

        if (cl[i] && !cr[i]) teams[i] = curr[i] == 'A' ? 'B' : 'A';
        else if (!cl[i] && cr[i]) teams[i] = curr[i];
    }
    cout << teams;
}
