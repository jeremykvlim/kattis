#include <bits/stdc++.h>
using namespace std;

void bfs(int u, int v, vector<vector<int>> &adj_list, vector<bool> &visited, vector<int> &count) {
    adj_list[u].emplace_back(v);
    if (visited[u]) {
        count[v]++;

        if (!visited[v]) {
            visited[v] = true;
            queue<int> q;
            q.emplace(v);
            while (!q.empty()) {
                v = q.front();
                q.pop();

                for (int w : adj_list[v]) {
                    count[w]++;

                    if (!visited[w]) {
                        visited[w] = true;
                        q.emplace(w);
                    }
                }
            }
        }
    }
}

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
    vector<bool> visited(n);
    vector<int> count(n);
    visited[0] = true;
    for (int i = 0; i < m; i++)
        if (!removed[i]) bfs(edges[i].first - 1, edges[i].second - 1, adj_list, visited, count);

    reverse(queries.begin(), queries.end());
    stack<int> references;
    for (auto &[c, x] : queries) {
        if (c == 1) bfs(edges[x - 1].first - 1, edges[x - 1].second - 1, adj_list, visited, count);
        else references.emplace(count[x - 1]);
    }

    while (!references.empty()) {
        cout << references.top() << "\n";
        references.pop();
    }
}
