#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int m;
        cin >> m;

        if (m == 1) {
            cout << "0\n";
            continue;
        }

        auto next = [&](int i, bool odd) {
            return (2 * i + odd) % m;
        };

        vector<pair<int, int>> dist(m, {INT_MAX, INT_MAX});
        vector<vector<int>> adj_list_regular(m), adj_list_transpose(m);
        priority_queue<pair<pair<int, int>, pair<int, int>>, vector<pair<pair<int, int>, pair<int, int>>>, greater<>> pq;
        pq.push({{1, 0}, {1, 1}});
        while (!pq.empty()) {
            auto [d, nodes] = pq.top();
            pq.pop();

            auto [u, v] = nodes;
            if (dist[u] < d) continue;

            adj_list_regular[u].emplace_back(v);
            adj_list_transpose[v].emplace_back(u);

            if (dist[u] == d) continue;

            auto [add, all] = dist[u] = d;
            if (dist[next(u, false)] > make_pair(add, all + 1)) pq.push({{add, all + 1}, {next(u, false), u}});
            if (dist[next(u, true)] > make_pair(add + 1, all + 1)) pq.push({{add + 1, all + 1}, {next(u, true), u}});
        }

        vector<bool> visited(m + 1, false);
        visited[0] = true;
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list_regular[v])
                if (!visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
        }

        stack<int> indices;
        indices.emplace(dist[0].second);
        for (int i = dist[0].second - 1, v = 1; ~i; i--)
            if (find(adj_list_transpose[v].begin(), adj_list_transpose[v].end(), next(v, false)) != adj_list_transpose[v].end() && visited[next(v, false)]) v = next(v, false);
            else {
                v = next(v, true);
                indices.emplace(i);
            }

        while (!indices.empty()) {
            cout << indices.top() << " ";
            indices.pop();
        }
        cout << "\n";
    }
}
