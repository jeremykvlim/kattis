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

    int d;
    cin >> d;

    vector<string> beers, alts;
    unordered_map<string, int> b, a;
    vector<pair<string, string>> dishes(d);
    for (auto &[beer, alt] : dishes) {
        cin >> beer >> alt;

        if (!b.count(beer)) {
            b[beer] = beers.size();
            beers.emplace_back(beer);
        }

        if (!a.count(alt)) {
            a[alt] = alts.size();
            alts.emplace_back(alt);
        }
    }

    int n = beers.size(), m = alts.size();
    vector<vector<int>> adj_list_b(n), adj_list_a(m);
    for (auto [beer, alt] : dishes) {
        int u = b[beer], v = a[alt];
        adj_list_b[u].emplace_back(v);
        adj_list_a[v].emplace_back(u);
    }

    vector<pair<int, int>> edges;
    for (int u = 0; u < adj_list_b.size(); u++)
        for (int v : adj_list_b[u]) edges.emplace_back(u, v);

    auto [l0, r0, matches0] = hopcroft_karp(n, m, edges);
    auto [cover_b0, cover_a0] = konig(n, m, adj_list_b, l0, r0);

    if (cover_b0.empty()) {
        for (int v = 0; v < n; v++) {
            if (adj_list_b[v].empty()) continue;

            edges.clear();
            vector<vector<int>> adj_list(n);
            for (int u = 0; u < n; u++)
                if (u != v) {
                    adj_list[u] = adj_list_b[u];
                    for (int t : adj_list[u]) edges.emplace_back(u, t);
                }

            auto [l1, r1, matches1] = hopcroft_karp(n, m, edges);
            if (matches1 + 1 == matches0) {
                auto [cover_b1, cover_a1] = konig(n, m, adj_list, l1, r1);
                if (!cover_a1.empty()) {
                    cover_b1.emplace_back(v);
                    tie(cover_b0, cover_a0) = tie(cover_b1, cover_a1);
                    goto done;
                }
            }
        }
        if (n) cover_b0.emplace_back(0);
    } else if (cover_a0.empty()) {
        for (int v = 0; v < m; v++) {
            if (adj_list_a[v].empty()) continue;

            edges.clear();
            vector<vector<int>> adj_list(n);
            for (int u = 0; u < n; u++)
                for (int t : adj_list_b[u])
                    if (t != v) {
                        adj_list[u].emplace_back(t);
                        edges.emplace_back(u, t);
                    }

            auto [l1, r1, matches1] = hopcroft_karp(n, m, edges);
            if (matches1 + 1 == matches0) {
                auto [cover_b1, cover_a1] = konig(n, m, adj_list, l1, r1);
                if (!cover_b1.empty()) {
                    cover_a1.emplace_back(v);
                    tie(cover_b0, cover_a0) = tie(cover_b1, cover_a1);
                    goto done;
                }
            }
        }
        if (m) cover_a0.emplace_back(0);
    }

    done:;
    cout << cover_b0.size() + cover_a0.size() << "\n";
    for (int v : cover_b0) cout << beers[v] << "\n";
    for (int v : cover_a0) cout << alts[v] << "\n";
}
