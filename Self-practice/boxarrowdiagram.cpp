#include <bits/stdc++.h>
using namespace std;


void bfs(int v, vector<vector<int>> &adj_list, vector<bool> &visited, vector<int> &counts) {
    visited[v] = true;
    queue<int> q;
    q.push(v);
    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (int u : adj_list[v]) {
            counts[u]++;
            if (!visited[u]) {
                visited[u] = true;
                q.push(u);
            }
        }
    }
}

void add(int u, int v, vector<vector<int>> &adj_list, vector<bool> &visited, vector<int> &counts) {
    adj_list[u].push_back(v);
    if (visited[u]) {
        counts[v]++;
        if (!visited[v]) bfs(v, adj_list, visited, counts);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj_list(n + 1);
    vector<pair<int, int>> edges(m + 1);
    vector<bool> visited(n + 1), removed(m + 1);
    vector<int> counts(n + 1);

    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
    }

    int q;
    cin >> q;
    vector<pair<int, int>> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].first >> queries[i].second;
        if (queries[i].first == 1) removed[queries[i].second] = true;
    }

    visited[1] = true;
    vector<int> answers;
    for (int i = 1; i <= m; i++)
        if (!removed[i]) add(edges[i].first, edges[i].second, adj_list, visited, counts);
    for (int i = q - 1; i >= 0; i--) {
        if (queries[i].first == 1) add(edges[queries[i].second].first, edges[queries[i].second].second, adj_list, visited, counts);
        else answers.emplace_back(counts[queries[i].second]);
    }

    for (int i = answers.size() - 1; i >= 0; i--)
        cout << answers[i] << "\n";
}
