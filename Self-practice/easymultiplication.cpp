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

        vector<pair<int, int>> dist(m, {INT_MAX, INT_MAX});
        vector<vector<int>> adj_list(m), transpose(m);
        priority_queue<pair<pair<int, int>, pair<int, int>>, vector<pair<pair<int, int>, pair<int, int>>>, greater<>> pq;
        pq.push({{1, 0}, {1, 1}});
        while (!pq.empty()) {
            auto [d, nodes] = pq.top();
            pq.pop();

            auto [v, u] = nodes;
            if (dist[v] < d) continue;

            adj_list[v].emplace_back(u);
            transpose[u].emplace_back(v);

            if (dist[v] == d) continue;

            auto [add, all] = dist[v] = d;
            if (dist[(2 * v) % m] > make_pair(add, all + 1)) pq.push({{add, all + 1}, {(2 * v) % m, v}});
            if (dist[(2 * v + 1) % m] > make_pair(add + 1, all + 1)) pq.push({{add + 1, all + 1}, {(2 * v + 1) % m, v}});
        }

        vector<bool> visited(m + 1, false);
        queue<int> q;
        q.emplace(0);
        visited[0] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (!visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
        }

        stack<int> indices;
        indices.emplace(dist[0].second);
        for (int i = dist[0].second - 1, v = 1; ~i; i--)
            if (find(transpose[v].begin(), transpose[v].end(), (2 * v) % m) != transpose[v].end() && visited[(2 * v) % m]) v = (2 * v) % m;
            else {
                v = (2 * v + 1) % m;
                indices.emplace(i);
            }

        while (!indices.empty()) {
            cout << indices.top() << " ";
            indices.pop();
        }
        cout << "\n";
    }
}
