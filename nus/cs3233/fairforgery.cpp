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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<int>> p(m, vector<int>(n)), count(n, vector<int>(n));
    for (auto &pi : p)
        for (int j = 0; j < n; j++) {
            cin >> pi[j];

            count[--pi[j]][j]++;
        }

    vector<vector<int>> forged(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int l = 0, t = 0, sum = 0; l < n; l++) {
            sum += count[i][l];
            while (t + 1 <= k && ((t + 1) * m + k - 1) / k <= sum) t++;
            forged[i][l] = t;
        }

    vector<int> used(n, 0);
    vector<vector<int>> indices(k, vector<int>(n, -1));
    for (int j = 0; j < n; j++)
        for (int l = j, tl = 0; l < n && tl < k; l++)
            for (int i = 0; i < n && tl < k; i++) {
                int tr = tl + max(min(k - tl, forged[i][l] - used[i]), 0);
                for (int t = tl; t < tr; t++) indices[t][j] = i;
                used[i] += tr - tl;
                tl = tr;
            }

    vector<pair<int, int>> edges;
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int t = 0; t < k; t++)
                if (indices[t][j] == i) {
                    adj_list[i].emplace_back(j);
                    edges.emplace_back(i, j);
                }

    vector<vector<int>> q(k, vector<int>(n, -1));
    for (int t = 0; t < k; t++) {
        auto [l, r, matches] = hopcroft_karp(n, n, edges);
        q[t] = r;

        edges.clear();
        for (int i = 0; i < n; i++)
            for (int j = 0; j < adj_list[i].size(); j++)
                if (q[t][adj_list[i][j]] == i) {
                    adj_list[i].erase(adj_list[i].begin() + j);
                    for (int v : adj_list[i]) edges.emplace_back(i, v);
                    break;
                }
    }

    for (auto qi : q) {
        for (int j : qi) cout << j + 1 << " ";
        cout << "\n";
    }
}
