#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    cin >> V >> E;

    auto sum = LLONG_MIN;
    vector<unordered_map<int, long long>> adj_matrix(V + 1);
    while (E--) {
        int a, b, s;
        cin >> a >> b >> s;

        if (adj_matrix[a].count(b)) {
            auto t = adj_matrix[a][b];
            adj_matrix[a][b] = adj_matrix[b][a] = max((long long) s, t);
            sum = max(sum, s + t);
        } else adj_matrix[a][b] = adj_matrix[b][a] = s;
    }

    queue<int> q;
    for (int i = 1; i <= V; i++)
        if (adj_matrix[i].size() <= 2) q.emplace(i);

    vector<bool> visited(V + 1, false);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (visited[v]) continue;
        visited[v] = true;

        if (adj_matrix[v].size() == 1) {
            auto [u, w] = *adj_matrix[v].begin();
            if (adj_matrix[u].size() <= 2) q.emplace(u);
        } else {
            auto [a, w1] = *adj_matrix[v].begin();
            auto [b, w2] = *next(adj_matrix[v].begin());
            adj_matrix[v].erase(a);
            adj_matrix[a].erase(v);
            adj_matrix[v].erase(b);
            adj_matrix[b].erase(v);

            auto s = w1 + w2;
            if (adj_matrix[a].count(b)) {
                auto t = adj_matrix[a][b];
                adj_matrix[a][b] = adj_matrix[b][a] = max((long long) s, t);
                sum = max(sum, s + t);
            } else adj_matrix[a][b] = adj_matrix[b][a] = s;
            if (adj_matrix[a].size() <= 2) q.emplace(a);
            if (adj_matrix[b].size() <= 2) q.emplace(b);
        }
    }
    cout << sum;
}
