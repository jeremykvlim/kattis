#include <bits/stdc++.h>
using namespace std;

void add(int u, int v, vector<vector<int>> &adj_list, vector<bool> &visited, vector<int> &count) {
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
                for (int u : adj_list[v]) {
                    count[u]++;
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                    }
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m;
    vector<vector<int>> adj_list(n + 1);
    vector<pair<int, int>> edges(m + 1);
    vector<bool> visited(n + 1), removed(m + 1);
    vector<int> count(n + 1);

    for (int i = 1; i <= m; i++) cin >> edges[i].first >> edges[i].second;

    cin >> q;
    vector<pair<int, int>> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].first >> queries[i].second;
        if (queries[i].first == 1) removed[queries[i].second] = true;
    }

    visited[1] = true;
    stack<int> references;
    for (int i = 1; i <= m; i++)
        if (!removed[i]) add(edges[i].first, edges[i].second, adj_list, visited, count);
    for (int i = q - 1; i >= 0; i--) {
        if (queries[i].first == 1) add(edges[queries[i].second].first, edges[queries[i].second].second, adj_list, visited, count);
        else references.emplace(count[queries[i].second]);
    }

    while (!references.empty()) {
        cout << references.top() << "\n";
        references.pop();
    }
}
