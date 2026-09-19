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

    int n, m;
    cin >> n >> m;

    vector<int> inspections(n);
    for (int &i : inspections) cin >> i;

    vector<vector<int>> time(n, vector<int>(n));
    for (auto &row : time) {
        for (int &t : row) cin >> t;

        for (int i = 0; i < n; i++) row[i] += inspections[i];
    }

    vector<array<int, 4>> flights(m);
    for (auto &fl : flights) {
        int s, f, t;
        cin >> s >> f >> t;

        fl = {s, f, t, t + time[s - 1][f - 1]};
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) time[j][k] = min(time[j][k], time[j][i] + time[i][k]);

    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            if (flights[i][1] == flights[j][0] && flights[i][3] <= flights[j][2] ||
                flights[i][3] + time[flights[i][1] - 1][flights[j][0] - 1] <= flights[j][2]) edges.emplace_back(i, j);


    auto [l, r, matches] = hopcroft_karp(m, m, edges);
    cout << m - matches;
}
