#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) cin >> u >> v;

    int q;
    cin >> q;

    vector<pair<int, int>> queries(q);
    vector<bool> removed(m);
    for (auto &[c, x] : queries) {
        cin >> c >> x;

        if (c == 1) removed[x - 1] = true;
    }

    vector<vector<int>> adj_list(n);
    vector<int> count(n, 0);
    vector<bool> visited(n, false);
    visited[0] = true;

    auto bfs = [&](int v, int u) {
        adj_list[v].emplace_back(u);
        if (visited[v]) {
            count[u]++;
            if (visited[u]) return;

            visited[u] = true;
            queue<int> q;
            q.emplace(u);
            while (!q.empty()) {
                u = q.front();
                q.pop();

                for (int t : adj_list[u]) {
                    count[t]++;

                    if (!visited[t]) {
                        visited[t] = true;
                        q.emplace(t);
                    }
                }
            }
        }
    };

    for (int i = 0; i < m; i++)
        if (!removed[i]) bfs(edges[i].first - 1, edges[i].second - 1);

    reverse(queries.begin(), queries.end());
    stack<int> counts;
    for (auto &[c, x] : queries) {
        if (c == 1) bfs(edges[x - 1].first - 1, edges[x - 1].second - 1);
        else counts.emplace(count[x - 1]);
    }

    while (!counts.empty()) {
        cout << counts.top() << "\n";
        counts.pop();
    }
}
